/* This file is part of the KDE libraries
    Copyright (C) 1997 Martin Jones (mjones@kde.org)
              (C) 1997 Torben Weis (weis@kde.org)
              (C) 1998 Waldo Bastian (bastian@kde.org)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/
//-----------------------------------------------------------------------------
//
// KDE HTML Widget - Tokenizers
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "htmltoken.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

#include <kcharsets.h>
#include <kapp.h>

// Include Java Script
#include <jsexec.h>

// Token buffers are allocated in units of TOKEN_BUFFER_SIZE bytes.
#define TOKEN_BUFFER_SIZE (32*1024)

static const char *commentStart = "<!--";
static const char *scriptEnd = "</script>";
static const char *styleEnd = "</style>";

//-----------------------------------------------------------------------------

const char *BlockingToken::tokenName()
{
    switch ( ttype )
    {
	case Table:
		return "</table";
		break;

	case FrameSet:
		return "</frameset";
		break;

	case Script:
		return "</script";
		break;

	case Cell:
		return "</cell";
		break;
    }

    return "";
}

//-----------------------------------------------------------------------------

HTMLTokenizer::HTMLTokenizer( KHTMLWidget *_widget )
{
    blocking.setAutoDelete( true );
    jsEnvironment = 0L;
    widget = _widget;
    last = next = curr = 0;
    buffer = 0;
}

void HTMLTokenizer::reset()
{
    while (!tokenBufferList.isEmpty())
    {
    	char *oldBuffer = (char *) tokenBufferList.take(0);
        delete [] oldBuffer;
    }

    last = next = curr = 0;
    tokenBufferSizeRemaining = 0; // No space allocated at all

    if ( buffer )
	delete [] buffer;
    buffer = 0;
}

void HTMLTokenizer::begin()
{
    reset();
    blocking.clear();
    size = 1000;
    buffer = new char[ 1024 ];
    dest = buffer;
    tag = false;
    pending = NonePending;
    discard = false;
    pre = false;
    prePos = 0;
    script = false;
    style = false;
    skipLF = false;
    select = false;
    comment = false;
    textarea = false;
    startTag = false;
    tquote = false;
    searchCount = 0;
    title = false;
    charEntity = false;
}

void HTMLTokenizer::addPending()
{
    if ( tag || select)     
    {
    	*dest++ = ' ';
    }
    else if ( textarea )
    {
	if (pending == LFPending)
	    *dest++ = '\n';
	else
	    *dest++ = ' ';    	
    }
    else if ( pre )
    {
    	int p;

	switch (pending)
	{
	  case SpacePending:
		// Insert a non-breaking space
		*(unsigned char *)dest++ = 0xa0; 
	  	prePos++;
	  	break;

	  case LFPending:
		if ( dest > buffer )
		{
		    *dest = 0;
		    appendToken( buffer, dest-buffer );
		}
		dest = buffer;
		*dest = TAG_ESCAPE;
		*(dest+1) = '\n';
		*(dest+2) = 0;
		appendToken( buffer, 2 );
		dest = buffer;
		prePos = 0; 
	  	break;
	  	
	  case TabPending:
		p = TAB_SIZE - ( prePos % TAB_SIZE );
		for ( int x = 0; x < p; x++ )
		{
		    *dest = ' ';
		    dest++;
		}
		prePos += p;
	  	break;
	  	
	  default:
	  	printf("Assertion failed: pending = %d\n", (int) pending);
	  	break;
	}
    }
    else
    {
    	*dest++ = ' ';
    }

    pending = NonePending;
}

void HTMLTokenizer::write( const char *str )
{
    // If this pointer is not 0L then we allocated some memory to store HTML
    // code in. This may happen while parsing the <script> tag, since the output
    // of the java code is treated as HTML code. This means we have to modify
    // the HTML code on the fly by inserting new HTML stuff.
    // If this pointer is not null, one has to free the memory before leaving
    // this function.
    char *srcPtr = 0L;

    KCharsets *charsets=KApplication::getKApplication()->getCharsets();
    
    if ( str == 0L || buffer == 0L )
	return;
    
    const char *src = str;

    while ( *src != 0 )
    {
	// do we need to enlarge the buffer?
	if ( dest - buffer > size )
	{
	    char *newbuf = new char [ size + 1024 + 20 ];
	    memcpy( newbuf, buffer, dest - buffer + 1 );
	    dest = newbuf + ( dest - buffer );
	    delete [] buffer;
	    buffer = newbuf;
	    size += 1024;
	}

	if (skipLF && (*src != '\n'))
	{
	    skipLF = false;
	}
	if (skipLF)
	{
	    src++;
	} 
	else if ( comment )
	{
	    // Look for '-->'
	    if (*src == '-') 
	    {
	        if (searchCount < 2)	// Watch out for '--->'
	            searchCount++;
	    }
	    else if ((searchCount == 2) && (*src == '>'))
	    {
	    	// We got a '-->' sequence
	    	comment = false;
	    }
	    else
	    {
	    	searchCount = 0;
	    }
            src++;
	}
	// We are inside of the <script> or <style> tag. Look for the end tag
	// which is either </script> or </style>,
	// otherwise print out every received character
	else if ( script || style )
	{
	    // Allocate memory to store the script. We will write maximal
	    // 10 characers.
	    if ( scriptCodeSize + 11 > scriptCodeMaxSize )
	    {
		char *newbuf = new char [ scriptCodeSize + 1024 ];
		memcpy( newbuf, scriptCode, scriptCodeSize );
		delete [] scriptCode;
		scriptCode = newbuf;
		scriptCodeMaxSize += 1024;
	    }

	    if ( ( *src == '>' ) && ( searchFor[ searchCount ] == '>'))
	    {
		src++;
		scriptCode[ scriptCodeSize ] = 0;
		scriptCode[ scriptCodeSize + 1 ] = 0;
		if (script) 
		{
		    script = false;
		    /* Parse scriptCode containing <script> info */
		    /* Not implemented */
		}
		else
		{
		    style = false;
		    /* Parse scriptCode containing <style> info */
		    /* Not implemented */
		}
	    }
	    // Find out wether we see a </script> tag without looking at
	    // any other then the current character, since further characters
	    // may still be on their way thru the web!
	    else if ( searchCount > 0 )
	    {
		if ( tolower(*src) == searchFor[ searchCount ] )
		{
		    searchBuffer[ searchCount ] = *src;
		    searchCount++;
		    src++;
		}
		// We were wrong => print all buffered characters and the current one;
		else
		{
		    searchBuffer[ searchCount ] = 0;
		    char *p = searchBuffer;
		    while ( *p ) scriptCode[ scriptCodeSize++ ] = *p++;
		    scriptCode[ scriptCodeSize++ ] = *src++;
		    searchCount = 0;
		}
	    }
	    // Is this perhaps the start of the </script> or </style> tag?
	    else if ( *src == '<' )
	    {
		searchCount = 1;
		searchBuffer[ 0 ] = '<';
		src++;
	    }
	    else
		scriptCode[ scriptCodeSize++ ] = *src++;
	}
	else if (charEntity)
	{
            int entityValue = 0;
	    QString res = 0;

	    searchBuffer[ searchCount] = *src;
	    searchBuffer[ searchCount+1] = '\0';
	    
	    // Check for '&#000' sequence
	    if (searchBuffer[1] == '#')
	    {
		if ((searchCount > 1) && 
		    (!isdigit(*src)))
	        {	
	    	    searchBuffer[ searchCount] = '\0';
	    	    entityValue = (int) strtol( &(searchBuffer[2]), 
	    	    				NULL, 10 );
	    	    charEntity = false;
	        }
	    }
	    else
	    {
	        // Check for &abc sequence
	        if (!isalpha(*src))
	        {
                    int len;
		    charEntity = false;
	    	    searchBuffer[ searchCount] = '\0';
		    res = charsets->convertTag(searchBuffer, len).copy();
		    if (len <= 0)
		    {
		    	res = 0;
		    }
	        }
	    }
	    
	    if (searchCount > 8)
	    {
	    	// This sequence is too long.. we ignore it
	        charEntity = false;
                memcpy(dest,searchBuffer, searchCount);
		dest += searchCount;
		*dest++ = *src++;
	    }
	    else if (charEntity)
	    {
	    	// Keep searching for end of character entity 	
	        searchCount++;
	        src++;
	    }
	    else
	    {
	    	
	    	// We have a complete sequence
	    	if (res && (res.length() == 1))
	    	{
	    	    entityValue = *((unsigned char *)res.data());
	    	}

		if (
		    (
		     (entityValue < 128) &&
		     (entityValue > 0)
		    ) 
		    ||
		    (entityValue == 160)
		   )
		{
		    // Just insert plain ascii
		    *dest++ = (char) entityValue;
		}	    	
		else if (!tag && !textarea && !select && !title) 
		{
		    // add current token first
		    if (dest > buffer)
		    {
		        *dest=0;
		        appendToken(buffer,dest-buffer);
		        dest = buffer;
		    }
		    
		    // add token with the amp-sequence for further conversion
		    appendToken(searchBuffer, searchCount);
		    dest = buffer;
		}
		else if (res)
		{
		    // insert the characters, assuming iso-8859-1
		    memcpy(dest, res.data(), res.length());
		    dest += res.length();
		}
		else if (entityValue > 0) 
		{
		    // insert the character, assuming iso-8859-1
		    *dest++ = (char) entityValue;
		}
		else
		{
		    // ignore the sequence, add it to the buffer as plaintext
		    memcpy(dest,searchBuffer, searchCount);
		    dest += searchCount;
		}
		if (*src == ';')
		    src++;
		searchCount = 0;
	    }
	}
	else if ( startTag)
	{
	    startTag = false;
	    if (*src == '/') 
	    {
	       // Start of an End-Tag 
	       pending = NonePending; // Ignore leading spaces 
	    }
	    else
	    {
	    	if (pending)
	    	    addPending();
	    }

	    if ( dest > buffer )
	    {
		*dest = 0;
		appendToken( buffer, dest-buffer );
		dest = buffer;
	    }
	    *dest = TAG_ESCAPE;
	    dest++;
	    *dest = '<';
	    dest++;
	    tag = true;
	    searchCount = 1; // Look for '<!--' sequence to start comment
	    // No 'src++' add '*src' in a second pass with 'startTag=false'
	}
	else if ( *src == '&' ) 
	{
            src++;
	    if ( pre )
		prePos++;	    
	    
	    discard = false; 
	    if (pending)
	    	addPending();
	    
	    charEntity = true;
            searchCount = 0;
            searchBuffer[searchCount++] = '&';
	}
	else if ( *src == '<' && !tag)
	{
	    src++;
	    startTag = true;
	    discard = false;
	}
	else if ( *src == '>' && tag && !tquote )
	{
            searchCount = 0; // Stop looking for '<!--' sequence

	    *dest = '>';
	    *(dest+1) = 0;

	    // make the tag lower case
	    char *ptr = buffer+2;
	    if (*ptr == '/')
	    { 
	    	// End Tag
	    	discard = false;
	    }
	    else
	    {
	    	// Start Tag
	    	// Ignore spaces & CR/LF's after a start tag
	    	discard = true;
	    }
	    while ( *ptr && *ptr != ' ' )
	    {
		*ptr = tolower( *ptr );
		ptr++;
	    }

	    appendToken( buffer, dest-buffer+1 );
	    dest = buffer;

	    tag = false;
	    pending = NonePending; // Ignore pending spaces
	    src++;

	    if ( strncmp( buffer+2, "pre", 3 ) == 0 )
	    {
		prePos = 0;
		pre = true;
	    }
	    else if ( strncmp( buffer+2, "/pre", 4 ) == 0 )
	    {
		pre = false;
	    }
	    else if ( strncmp( buffer+2, "textarea", 8 ) == 0 )
	    {
		textarea = true;
	    }
	    else if ( strncmp( buffer+2, "/textarea", 9 ) == 0 )
	    {
		textarea = false;
	    }
	    else if ( strncmp( buffer+2, "title", 5 ) == 0 )
	    {
		title = true;
	    }
	    else if ( strncmp( buffer+2, "/title", 6 ) == 0 )
	    {
		title = false;
	    }
	    else if ( strncmp( buffer+2, "script", 6 ) == 0 )
	    {
		script = true;
                searchCount = 0;
                searchFor = scriptEnd;		
		scriptCode = new char[ 1024 ];
		scriptCodeSize = 0;
		scriptCodeMaxSize = 1024;
	    }
	    else if ( strncmp( buffer+2, "style", 5 ) == 0 )
	    {
		style = true;
                searchCount = 0;		
                searchFor = styleEnd;		
		scriptCode = new char[ 1024 ];
		scriptCodeSize = 0;
		scriptCodeMaxSize = 1024;
	    }
	    else if ( strncmp( buffer+2, "select", 6 ) == 0 )
	    {
		select = true;
	    }
	    else if ( strncmp( buffer+2, "/select", 7 ) == 0 )
	    {
		select = false;
	    }
	    else if ( strncmp( buffer+2, "frameset", 8 ) == 0 )
	    {
		blocking.append( new BlockingToken( BlockingToken::FrameSet,
				last ) );
	    }
	    else if ( strncmp( buffer+2, "cell", 4 ) == 0 )
	    {
		blocking.append( new BlockingToken(BlockingToken::Cell, last) );
	    }
	    else if ( strncmp( buffer+2, "table", 5 ) == 0 )
	    {
		blocking.append( new BlockingToken( BlockingToken::Table,
				last ) );
	    }
	    else if ( !blocking.isEmpty() && 
		    strncasecmp( buffer+1, blocking.getLast()->tokenName(),
			strlen( blocking.getLast()->tokenName() ) ) == 0 )
	    {
		blocking.removeLast();
	    }
	}
	else if (( *src == '\n' ) || ( *src == '\r' ) ||
	         ( *src == ' ' ) || ( *src == '\t'))
	{
	    if ( tquote)
	    {
	    	*dest++ = *src; // Just add it
	    }
	    else if ( tag )
	    {
                searchCount = 0; // Stop looking for '<!--' sequence
		if (!discard)
		    pending = SpacePending;
	    }
	    else if ( pre)
	    {
	    	if (!discard)
	    	{
	    	    if (pending)
	    	        addPending();
	    	    if (*src == ' ')
	    	        pending = SpacePending;
	    	    else if (*src == '\t')
	    	        pending = TabPending;
	    	    else
	    	    	pending = LFPending;
	    	}
	    	else
	    	{
	    	    discard = false; // We have discarded 1 white-space
	    	}
	    } 
	    else
	    {
	    	if (!discard)
	    	    pending = SpacePending;
	    }
	    /* Check for MS-DOS CRLF sequence */
	    if (*src == '\r')
	    {
		skipLF = true;
	    }
	    src++;
	}
	else if ( *src == '\"' || *src == '\'')
	{ // we treat " & ' the same in tags
    	    discard = false;
	    if ( tag )
	    {
	        searchCount = 0; // Stop looking for '<!--' sequence
		src++;
		if ( *(dest-1) == '=' && !tquote )
		{
		    pending = NonePending;
		    tquote = true;
		    *dest++ = '\"';
		}
		else if ( tquote )
		{
		    tquote = false;
		    *dest++ = '\"';
		    pending = SpacePending; // Add space automatically
		}
		else
		{
		    // Ignore stray "\'"
		}
	    }
	    else
	    {
	    	if (pending)
	    	    addPending();

		if ( pre )
		    prePos++;

		*dest++ = *src++;
	    }
	}
	else if ( *src == '=' )
	{
	    src++;
	    discard = false;
	    if ( tag )
	    {
	        searchCount = 0; // Stop looking for '<!--' sequence
                *dest++ = '=';
		if ( !tquote )
		{
		    pending = NonePending; // Discard spaces before '='
		    discard = true; // Ignore following spaces
		}
	    }
	    else
	    {
	    	if (pending)
	    	    addPending();

		if ( pre )
		    prePos++;

		*dest++ = '=';
	    }
	}
	else
	{
	    discard = false;
	    if (pending)
	    	addPending();

	    if (tag)
	    {
	      if (searchCount > 0)
	      {
	    	if (*src == commentStart[searchCount])
	    	{
	    	    searchCount++;
	    	    if (searchCount == 4)
	    	    {
	    	    	// Found '<!--' sequence
	    	        comment = true;
		        dest = buffer; // ignore the previous part of this tag
		        tag = false;
		        searchCount = 0;
		        continue;
	    	    }
	    	}
	    	else
	    	{
	            searchCount = 0; // Stop looking for '<!--' sequence
                }  
              }
	    } 
	    else if ( pre )
	    {
		prePos++;
	    }
	    
	    *dest++ = *src++;
	}
    }

    if ( srcPtr )
	delete [] srcPtr;
}

void HTMLTokenizer::end()
{
    if ( buffer == 0 )
	return;

    if ( dest > buffer )
    {
	*dest = 0;
	appendToken( buffer, dest-buffer );
    }

    delete [] buffer;
    buffer = 0;

    // if there are still blocking tokens then the HTML is illegal - remove
    // blocks anyway and hope for the best
    blocking.clear();
}

void HTMLTokenizer::appendTokenBuffer( int min_size)
{
    int newBufSize = TOKEN_BUFFER_SIZE; 

    // If we were using a buffer, mark it's end
    if (next)
    {
       // Mark current buffer end
       *next = '\0';
    }

    if (min_size > newBufSize)
    {
        // Wow! This surely is a big token...
        newBufSize += min_size; 
    }
    HTMLTokenBuffer *newBuffer = (HTMLTokenBuffer *) new char [ newBufSize ];
    tokenBufferList.append( newBuffer);
    next = newBuffer->first();
    tokenBufferSizeRemaining = newBufSize;
    if (!curr)
    {
       curr = tokenBufferList.at(0)->first();
       tokenBufferCurrIndex = 0;
    }
}                                                                                         

void HTMLTokenizer::nextTokenBuffer()
{
    tokenBufferCurrIndex++;
    if (tokenBufferCurrIndex < tokenBufferList.count())
    {
        curr = tokenBufferList.at(tokenBufferCurrIndex)->first();
    }
    else 
    {
        // Should never occur.
        printf("ERROR in HTMLTokenize::nextToken()\n");
    }
}

void HTMLTokenizer::first()
{ 
    tokenBufferCurrIndex = 0;
    curr = 0;
    if (tokenBufferList.count())
    {
        HTMLTokenBuffer *tokenBufferCurr = tokenBufferList.at(tokenBufferCurrIndex);
        if (tokenBufferCurr)
        {
            curr = tokenBufferCurr->first();
        }
    }
}

HTMLTokenizer::~HTMLTokenizer()
{
    reset();
}

//-----------------------------------------------------------------------------

StringTokenizer::StringTokenizer()
{
    buffer = 0;
    pos    = 0;
    end    = 0;
    bufLen = 0;
}

void StringTokenizer::tokenize( const char *str, const char *_separators )
{
    if ( *str == '\0' )
    {
	pos = 0;
	return;
    }

    int strLength = strlen( str ) + 1;

    if ( bufLen < strLength )
    {
	delete [] buffer;
	buffer = new char[ strLength ];
	bufLen = strLength;
    }

    const char *src = str;
    end = buffer;
    bool quoted = false;
    
    for ( ; *src != '\0'; src++ )
    {
	char *x = strchr( _separators, *src );
	if ( *src == '\"' )
	    quoted = !quoted;
	else if ( x && !quoted )
	    *end++ = 0;
	else
	    *end++ = *src;
    }

    *end = 0;

    if ( end - buffer <= 1 )
	pos = 0;	// no tokens
    else
	pos = buffer;
}

const char* StringTokenizer::nextToken()
{
    if ( pos == 0 )
	return 0;

    char *ret = pos;
    pos += strlen( ret ) + 1;
    if ( pos >= end )
	pos = 0;

    return ret;
}

StringTokenizer::~StringTokenizer()
{
    if ( buffer != 0 )
	delete [] buffer;
}
