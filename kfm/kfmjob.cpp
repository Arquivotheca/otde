#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stddef.h>
#include <sys/stat.h>

#include <qmsgbox.h>

#include "kfmjob.h"
#include "kbind.h"
#include "config-kfm.h"
#include "kmsgwin.h"
#include "kfmdlg.h"
#include "kmimemagic.h"
#include "kcookiejar.h"
#include "kioslave/kio_errors.h"

#include <klocale.h>

int tmpFileCounter = 0;

KFMJob::KFMJob( )
{
    job = 0L;    
    bRunning = FALSE; 
    bError = false;
}

bool KFMJob::browse( const char *_url, bool _reload, bool _bHTML, const char *_currentURL, QList<KIODirectoryEntry> *_list, 
                     const char *_data)
{
    bError = false;

    KURL u( _url );
    if ( u.isMalformed() )
    {
	warning(klocale->translate("ERROR: Malformed URL"));
	return false;
    }

    url = _url;
    post_data = _data;
    
    if ( job )
	delete job;	
    job = 0L;

    // We are not shure yet
    isDir = FALSE;
    isHTML = FALSE;
    bCheckedMimeType = FALSE;
    bFinished = FALSE;
    bRunning = TRUE;
    f = 0L;
    
    bool isFile = FALSE;
    // Lets have a look at what we already know about the URL.
    if ( _currentURL && _list && strncmp( _currentURL, _url, strlen( _currentURL ) ) == 0L )
    {
	// Get the current directory
	KURL u2( _currentURL );
	QString d = u2.directoryURL();
	
	KIODirectoryEntry *e;
	for ( e = _list->first(); e != 0L; e = _list->next() )
	{
	    QString str = d.data();
	    str += e->getName();
	    if ( str == _url )
		isFile = e->isFile();
	}
    }
    
    // A HACK
    // Now we clear the list since it may happen that we emit 'newDirEntry' before we
    // return.
    if ( _list )
	_list->clear();
    
    // Can we get a directory listing with the protocol or do we know that it is a file ?
    // Or does KIOServer know that it is a file
    if ( KIOServer::getKIOServer()->supports( _url, KIO_List ) && !isFile && KIOServer::isDir( _url ) != 0 )
    {
	if ( url.right( 1 ) != "/" && u.hasPath() )
	    url += "/"; 

	// Lets try to load it as directory
	bFileLoad = FALSE;
	job = new KIOJob;
	job->setAutoDelete( FALSE );
	job->display( false );
	connect( job, SIGNAL( errorFilter( int, const char*,int  ) ), this, SLOT( slotError( int, const char*, int ) ) );
	connect( job, SIGNAL( newDirEntry( int, KIODirectoryEntry* ) ),
		 this, SLOT( slotNewDirEntry( int, KIODirectoryEntry* ) ) );
	connect( job, SIGNAL( finished( int ) ), this, SLOT( slotFinished( int ) ) );
	connect( job, SIGNAL( data( const char*, int ) ), this, SLOT( slotDirHTMLData( const char*, int ) ) );
	connect( job, SIGNAL( redirection( const char* ) ), this, SLOT( slotRedirection( const char* ) ) );
	connect( job, SIGNAL( cookie( const char*, const char* ) ), this, SLOT( slotCookie( const char*, const char* ) ) );
	connect( job, SIGNAL( info( const char* ) ), this, SLOT( slotInfo( const char* ) ) );

	// If we get HTML, then slotDirHTMLData will receive the HTML code.
	job->list( url, _reload, _bHTML );
    }
    else
	openFile(_reload);
    
    return true;
}

void KFMJob::openFile(bool _reload)
{
    isDir = FALSE;
    
    // OK, we try to load the file
    job = new KIOJob;
    job->setAutoDelete( FALSE );
    job->display( false );
    connect( job, SIGNAL( finished( int ) ), this, SLOT( slotFinished( int ) ) );
    connect( job, SIGNAL( errorFilter( int, const char*, int ) ), this, SLOT( slotError( int, const char*, int ) ) );
    connect( job, SIGNAL( data( const char*, int ) ), this, SLOT( slotData( const char*, int ) ) );
    connect( job, SIGNAL( mimeType( const char* ) ), this, SLOT( slotMimeType( const char* ) ) );
    connect( job, SIGNAL( redirection( const char* ) ), this, SLOT( slotRedirection( const char* ) ) );
    connect( job, SIGNAL( cookie( const char*, const char* ) ), this, SLOT( slotCookie( const char*, const char* ) ) );
    connect( job, SIGNAL( info( const char* ) ), this, SLOT( slotInfo( const char* ) ) );
    
    // Delete the trailing / since we assume that the URL is a file.
    // But dont delete the root '/', since for example "file:" is considered
    // malformed.
    if ( url.left(4) == "file" && url.right(1) == "/" && url.right(2) != ":/" )
	url = url.left( url.length() - 1 );
    
    bFileLoad = TRUE;

    if (!cookiejar)
    {
        printf("Making cookiejar....\n");
        cookiejar = new KCookieJar;
    }

    cookie_data = cookiejar->findCookies(url);

//@@WABA Look out for cookies!
    printf("KFMJob::openFile: URL=%s : \n---%s---\n", url.data(), cookie_data.data());
    job->get( url, _reload, post_data.data(), cookie_data.data() );
}

void KFMJob::slotRedirection( const char *_url )
{
    url = _url;
    emit redirection( _url );
    openFile( false);
}

void KFMJob::slotCookie( const char *_url, const char *_cookie_str )
{
    emit cookie( _url, _cookie_str );
}

void KFMJob::slotInfo( const char *_text )
{
    emit info( _text );
}

void KFMJob::slotDirHTMLData( const char *_data, int _len )
{
    // Did we already check the mimetype ?
    if ( !bCheckedMimeType )
    {
	// This slot is called due to "job->list".
	// If we get a call we can assume that the URL is indeed a directory
	// and directories are displayed using HTML.
	isDir = TRUE;
	// Tell our client about the mime type.
	emit mimeType( "text/html" );
	bCheckedMimeType = TRUE;
    }
    
    emit data( _data, _len );
}

void KFMJob::slotError( int _kioerror, const char *_text, int _errno )
{
    // We tried something that the protocol did not support
    if ( _kioerror == KIO_ERROR_NotPossible || _kioerror == KIO_ERROR_NotADirectory )
    {
	// Was the unsupported action perhaps some 'list' command ?
	// Perhaps we tried to list a gzip file or something
	// stupid like that ?
	if ( !bFileLoad && !isDir )
	{
	    // Stop the running job, since it is nonsense
	    disconnect( job, 0, this, 0 );
	    if ( !bFinished )
	      job->cancel();
	    job->setAutoDelete( TRUE );
	    job = 0L;
	    // Try to open as file
	    openFile(false);
	    return;
	}
    }

    bError = true;
    
    // Allow the job to print an error message.
    job->display( true );
    // Process the error
    job->processError( _kioerror, _text, _errno );
    
    // Tell our client about it
    emit error( _kioerror, _text );
}

void KFMJob::slotNewDirEntry( int , KIODirectoryEntry * _entry )
{
    // Now we know that it is a directory
    isDir = TRUE;
    emit newDirEntry( _entry );
}

void KFMJob::slotMimeType( const char *_type )
{
    // Tell our client that the protocol knows about
    // the mimetype. This is usually HTTP.
    bCheckedMimeType = TRUE;
    emit mimeType( _type );
}

void KFMJob::slotData( const char *_text, int _len )
{
    if ( !bCheckedMimeType )
    {
	// dataBuffer += _text;
	// if ( dataBuffer.length() )
	testMimeType( _text, _len );
	return;
    }
    
    emit data( _text, _len );
}

void KFMJob::testMimeType( const char *_text, int _len )
{
    bCheckedMimeType = TRUE;
    
    KMimeMagicResult *result = KMimeType::findBufferType( _text, _len );
    
    if ( strcmp( "text/html", result->getContent() ) == 0 ) 
    {
	isHTML = TRUE;
	emit mimeType( "text/html" );
	emit data( _text, _len );
    }
    else if ( result->getAccuracy() > 0 )
    {
	emit mimeType( result->getContent() );
    }
    else
    {
	// debugT("UNKNOWN TYPE\n");
	// Dont know the mime type
	emit mimeType( 0L );
    }
}

void KFMJob::slotFinished( int )
{
    bFinished = TRUE;
    
    // Did we only try to load the URL as directory and did we get 0 entries ?
    /* if ( !bFileLoad && !isDir )
    {
	disconnect( job, 0, this, 0 );
	job->setAutoDelete( TRUE );
	job = 0L;
	// Try to open as file
	openFile();
	return;
    } */

    // Are we loading a file ?
    if ( bFileLoad && !bError )
    {
	// Did we check for the mime type already ?
	if ( !bCheckedMimeType )
	{
	    // Since we did not get any data ...
	    emit mimeType( "application/x-zerosize" );
	    //testMimeType( dataBuffer );
	}
    } 

    disconnect( job, 0, this, 0 );
    if (job)
      job->setAutoDelete( TRUE );
    job = 0L;

    if ( !bError )
      emit finished();

    bRunning = FALSE;
}

void KFMJob::stop()
{
    if ( !bRunning )
	return;
    
    bRunning = FALSE;
   
    if ( job )
    {
	disconnect( job, 0, this, 0 );
	job->setAutoDelete( TRUE );
	if ( !bFinished )
	    job->cancel();
	job = 0L;
    }

    bFinished = TRUE;
}

const char* KFMJob::getURL()
{
    return url;
}

KFMJob::~KFMJob()
{
    stop();
}


#include "kfmjob.moc"