#include "file.h"

#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>

#include <qregexp.h>

#include <qdir.h>

SortedKProtocolDirEntry::~SortedKProtocolDirEntry()
{
}

int SortedKProtocolDirEntry::compareItems( GCI i1, GCI i2)
{
    KProtocolDirEntry *de1 = (KProtocolDirEntry *)i1;
    KProtocolDirEntry *de2 = (KProtocolDirEntry *)i2;
    if (sortMode & DIRSORT_DIRSFIRST)
    {
	if (de1->isdir != de2->isdir)
	    return ((de1->isdir) ? -1 : 1);
    }
    switch (sortMode & 0x7f)
    {
	case DIRSORT_BYNAME:
	    return strcmp( de1->name.data(), de2->name.data() );
	case DIRSORT_BYSIZE:
	    return de1->size - de2->size;
	default:
	    return strcmp( de1->name.data(), de2->name.data() );
    }
}

KProtocolFILE::KProtocolFILE()
{
	file = NULL;
	dlist = 0L;
	de = 0L;
	bKdeHtml = FALSE;
	sortMode = (DIRSORT_DIRSFIRST | DIRSORT_BYNAME);
	allowHTML = FALSE;
}

KProtocolFILE::~KProtocolFILE()
{
	Close();
        delete dlist;
}

int KProtocolFILE::Open(KURL *url, int mode)
{
    QString p( url->path() );
    KURL::decodeURL( p );
    
	struct stat st;
	Close();

	size=0;

	if(mode & READ)
	{

		file = fopen( p, "rb");
		if(!file)
		{
			Error(KIO_ERROR_CouldNotRead,"Can't open File for reading",errno);
			return(FAIL);
		}
		fstat(fileno(file), &st);
		size = st.st_size;
	}
	if(mode & WRITE)
	{
		if(!(mode & OVERWRITE))
		{
			if(stat( p, &st) != -1)
			{
			    Error(KIO_ERROR_FileExists,"File already exists",errno);
				return(FAIL);
			}
		}

		file = fopen( p, "wb");
		if(!file)
		{
			Error(KIO_ERROR_CouldNotRead,"Can't open file for reading",errno);
			return(FAIL);
		}
	}
	return SUCCESS;
}

int KProtocolFILE::Close()
{
	if(file) fclose(file);
	file = NULL;
	return SUCCESS;
}

long KProtocolFILE::Read(void *buffer, long len)
{
	long n = fread(buffer,1,len,file);
	if(n < 0)
	{
		Error(KIO_ERROR_CouldNotRead,"Reading from file failed",errno);
		return(FAIL);
	}
	return(n);
}

long KProtocolFILE::Write(void *buffer, long len)
{
	long n = fwrite(buffer,1,len,file);
	if(n < 0)
	{
		Error(KIO_ERROR_CouldNotRead,"Writing to file failed",errno);
		return(FAIL);
	}
	return(n);
}

long KProtocolFILE::Size()
{
	if(!file) return(FAIL);
	return(size);
}

int KProtocolFILE::atEOF()
{
	if(!file) return(FAIL);
	return(feof(file));
}

int KProtocolFILE::OpenDir( KURL *url )
{
    file = 0L;
    DIR *dp = 0L;
    struct dirent *ep;
    struct stat buff;
    
    // Open the directory
    path = url->path();
    KURL::decodeURL(path);
    // Save a copy
    QString tmp = path.data();
    if ( path.right(1) != "/" )
	path += "/";

    // Are we allows to respond with HTML ?
    if ( allowHTML )
    {
	// Try to open .kde.html
	FILE *f;
	QString t( path.data() );
	t += ".kde.html";
	if (( stat( t.data(), &buff ) == 0 ) && S_ISREG( buff.st_mode ))
	    if ((f = fopen( t.data(), "rb" )))
	    {
		kdeHtmlSize = buff.st_size;
		bKdeHtml = TRUE;
		file = f;
		return SUCCESS;
	    }

	// Try to open index.html
	t = path.data();
	t += "index.html";
	if (( stat( t.data(), &buff ) == 0 ) && S_ISREG( buff.st_mode ))
	    if (( f = fopen( t.data(), "rb" )))
	    {
		file = f;
		return SUCCESS;
	    }
    }
    
    de = 0L;
    dp = opendir( tmp );
    if ( dp == 0L )
    {
	Error(KIO_ERROR_CouldNotRead,"Could not enter directory",errno);
	return FAIL;
    }
    
    if ( path.right(1) != "/" )
	path += "/";

    dlist = new SortedKProtocolDirEntry();
    dlist->setAutoDelete( true );
    dlist->sortMode = sortMode;

    while ( ( ep = readdir( dp ) ) != 0L )
    {
    	QString name(ep->d_name);
	KURL::encodeURL(name);

	QString fname = path.data();
	fname += ep->d_name;

	int stat_ret = stat( fname, &buff );
	struct stat lbuff;
	/* int lstat_ret = */ lstat( fname, &lbuff );
	struct tm *t = localtime( &lbuff.st_mtime );
	struct passwd * user = getpwuid( buff.st_uid );
	struct group * grp = getgrgid( buff.st_gid );

	char buffer[1024];
	KProtocolDirEntry *_de = new KProtocolDirEntry();
	
	_de->isdir = FALSE;
	if ( S_ISLNK( lbuff.st_mode ) )
	    buffer[0] = 'l';		
	else if ( S_ISDIR( buff.st_mode ) )
	{
	    buffer[0] = 'd';
	    _de->isdir = TRUE;
	}
	else
	    buffer[0] = '-';
	
	char uxbit,gxbit,oxbit;
 
	if ( (buff.st_mode & (S_IXUSR|S_ISUID)) == (S_IXUSR|S_ISUID) )
	    uxbit = 's';
	else if ( (buff.st_mode & (S_IXUSR|S_ISUID)) == S_ISUID )
	    uxbit = 'S';
	else if ( (buff.st_mode & (S_IXUSR|S_ISUID)) == S_IXUSR )
	    uxbit = 'x';
	else
	    uxbit = '-';
	
	if ( (buff.st_mode & (S_IXGRP|S_ISGID)) == (S_IXGRP|S_ISGID) )
	    gxbit = 's';
	else if ( (buff.st_mode & (S_IXGRP|S_ISGID)) == S_ISGID )
	    gxbit = 'S';
	else if ( (buff.st_mode & (S_IXGRP|S_ISGID)) == S_IXGRP )
	    gxbit = 'x';
	else
	    gxbit = '-';
	
	if ( (buff.st_mode & (S_IXOTH|S_ISVTX)) == (S_IXOTH|S_ISVTX) )
	    oxbit = 't';
	else if ( (buff.st_mode & (S_IXOTH|S_ISVTX)) == S_ISVTX )
	    oxbit = 'T';
	else if ( (buff.st_mode & (S_IXOTH|S_ISVTX)) == S_IXOTH )
	    oxbit = 'x';
	else
	    oxbit = '-';
    
	buffer[1] = ((( buff.st_mode & S_IRUSR ) == S_IRUSR ) ? 'r' : '-' );
	buffer[2] = ((( buff.st_mode & S_IWUSR ) == S_IWUSR ) ? 'w' : '-' );
	buffer[3] = uxbit;
	buffer[4] = ((( buff.st_mode & S_IRGRP ) == S_IRGRP ) ? 'r' : '-' );
	buffer[5] = ((( buff.st_mode & S_IWGRP ) == S_IWGRP ) ? 'w' : '-' );
	buffer[6] = gxbit;
	buffer[7] = ((( buff.st_mode & S_IROTH ) == S_IROTH ) ? 'r' : '-' );
	buffer[8] = ((( buff.st_mode & S_IWOTH ) == S_IWOTH ) ? 'w' : '-' );
	buffer[9] = oxbit;
	buffer[10] = 0;

	// A link pointing to nowhere ?
	if ( stat_ret == -1 && S_ISLNK( lbuff.st_mode ) )
	    strcpy( buffer, "lrwxrwxrwx" );
	
	_de->access = buffer;
	_de->name = name.data();
	_de->owner = (( user != 0L ) ? user->pw_name : "???" );
	_de->group = (( grp != 0L ) ? grp->gr_name : "???" );
	QString d;
	d.sprintf("%02i:%02i %02i.%02i.%02i", t->tm_hour,t->tm_min,t->tm_mday,t->tm_mon + 1,t->tm_year );
	_de->date = d.data();
	_de->size = buff.st_size;
	if ( _de->isdir )
	    _de->name += "/";

	if ( sortMode == DIRSORT_NONE )
	    dlist->append( _de );
	else
	    dlist->inSort( _de );
    }
    closedir( dp );
    de = dlist->first();

    return SUCCESS;
}

void KProtocolFILE::EmitData( KIOSlaveIPC *_ipc )
{
    if ( bKdeHtml )
    {
	OpenKdeHtml( _ipc );
	return;
    }
    
    // Read all data from 'file'
    char buffer[ 4100 ];
    while ( !feof( file ) )
    {
	int n = fread( buffer, 1, 4096, file );
	if ( n > 0 )
	{
	    buffer[n] = 0;
	    _ipc->data( IPCMemory( buffer, n ) );
	}
    }
}

KProtocolDirEntry *KProtocolFILE::ReadDir()
{
    // Loop thru all directory entries

    if (de) {
	KProtocolDirEntry *ode = de;
	de = dlist->next();
	return ode;
    }
    return NULL;
}

int KProtocolFILE::CloseDir()
{
    if ( file )
	fclose( file );
    file = 0;
    return SUCCESS;
}

int KProtocolFILE::MkDir(KURL *url)
{
    QString p( url->path() );
    KURL::decodeURL( p );
    
    if ( ::mkdir( p, S_IXUSR | S_IWUSR | S_IRUSR ) != 0L )
	return Error( KIO_ERROR_CouldNotMkdir, "Could not mkdir '%s'", errno );
    return SUCCESS;
}

int KProtocolFILE::GetPermissions( KURL &_u )
{
    QString p( _u.path() );
    KURL::decodeURL( p );
    
    struct stat buff;
    stat( p, &buff );

    return (int)(buff.st_mode);
}

void KProtocolFILE::SetPermissions( KURL &_u, int _perms )
{
    if ( _perms == -1 )
	return;
    
    chmod( _u.path(), (mode_t)_perms );                     
}

bool KProtocolFILE::OpenKdeHtml( KIOSlaveIPC *_ipc )
{
    QList<KProtocolDirEntry> list;
    list.setAutoDelete( FALSE );
    
    KProtocolDirEntry *de;
    KProtocolFILE prot;

    KURL u( path );
    
    prot.AllowHTML( FALSE );
    if ( prot.OpenDir( &u ) == KProtocol::FAIL )
    {
	int kerror;
	QString msg;
	int syserror;
	prot.GetLastError( kerror, msg, syserror );
	Error( kerror, msg, syserror );
	return FAIL;
    }

    // Read every directory entry
    while( ( de = prot.ReadDir() ) )
	list.append( de );
    prot.CloseDir();

    // Read the HTML code into memory
    char *mem = (char*) malloc( kdeHtmlSize + 1 );
    fread( mem, 1, kdeHtmlSize, file );
    mem[ kdeHtmlSize ] = 0;
	
    // Stores all files we already displayed.
    // This is used for filter option "Rest"
    QList<QString> displayedFiles;
    displayedFiles.setAutoDelete( TRUE );
    // Indicates wether filter option "Rest" is currently selected.
    bool rest = FALSE;

    char *p = mem;
    char *old = mem;

    // Search All <files...> tags
    while ( ( p = strstr( p, "<files " ) ) != 0L )
    {
	rest = FALSE;
	
	*p = 0;
	// Write everything up to the start of <files...
        _ipc->data( IPCMemory( old, strlen( old ) ) );
	// Make the <files...> tag a 0 terminated string
	char *begin = p + 1;
	p = strchr( p + 1, '>' );
	if ( p == 0 )
	    p = mem + strlen( mem );
	*p = 0;
	{
	    // RegExpr. for wildcard pattern
	    QRegExp re;
	    // Wildcard pattern
	    QString name;
	    // Filter flags
	    int filter = 0;
	    
	    // Delete the "<files " part of the string
	    QString str = (const char*) begin + 6;
	    str = str.simplifyWhiteSpace();
	    // Parse the arguments of the <files...> tag
	    // Look at each argument
	    while ( str.length() > 0 )
	    {
		QString token;
		// Extract the token
		int i = str.find( ' ' );
		if ( i == -1 )
		{
		    token = str.data();
		    str = "";
		}
		else
		{
		    token = str.left( i ).data();
		    str = str.data() + i + 1;
		}
		
		if ( strncasecmp( token, "name=", 5 ) == 0)
		{
		    name = token.data() + 5;
		    // A regular expression
		    if ( name.data()[0] == '/' )
		    {
			re = name.data() + 1;
			re.setWildcard( FALSE );
		    }
		    // a wildcard pattern
		    else
		    {
			re = name.data();
			re.setWildcard( TRUE );
		    }
		}
		else if (strncasecmp( token, "filter=", 7 ) == 0)
		{		    
		    QString s( token.data() + 7 );
		    
		    int j = 0;
		    while ( j < (int)s.length() )
		    {
			if ( strncasecmp( s.data() + j, "Dirs", 4 ) == 0 )
			{
			    filter |= QDir::Dirs;
			    j += 4;
			}
			else if ( strncasecmp( s.data() + j, "Files", 5 ) == 0 )
			{
			    filter |= QDir::Files;
			    j += 5;
			}
			else if ( strncasecmp( s.data() + j, "Drives", 6 ) == 0 )
			{
			    filter |= QDir::Drives;
			    j += 6;
			}
			else if ( strncasecmp( s.data() + j, "Executable", 10 ) == 0 )
			{
			    filter |= QDir::Executable;
			    j += 10;
			}
			else if ( strncasecmp( s.data() + j, "Hidden", 6 ) == 0 )
			{
			    filter |= QDir::Hidden;
			    j += 6;
			}
			else if ( strncasecmp( s.data() + j, "Rest", 4 ) == 0 )
			{
			    rest = TRUE;
			    // filter = 0;
			    j += 4;
			}
			else
			{
			    j = s.find( '|', j );
			    if ( j == -1 )
				j = s.length();
			}
			
			// Skip the separator
			while ( s[ j ] == '|' ) j++;
		    }
		}
	    }

	    // The name pattern. Default is no pattern
	    const char *pname = 0L;
	    // Do we have a pattern ?
	    if ( !name.isNull() )
		if ( name.data()[0] != 0 )
		    pname = name.data();

	    char buff [ 1024 ];
	    
	    // Traverse all files
	    for ( de = list.first(); de != 0L; de = list.next() )
	    {
		if ( de->name != "./" )
		{
		    bool ok = FALSE;
		    bool ok2 = TRUE;
		    QString *s;
		    // Have we displayed this file already?
		    for ( s = displayedFiles.first(); s != 0L; s = displayedFiles.next() )
			if ( strcmp( s->data(), de->name ) == 0 )
			    ok2 = FALSE;
		 
		    if ( de->name == ".." )
		    {
			KURL u2( u.path() );
			u2.cdUp();
			strcpy( buff, "file:" );
			strcat( buff, u2.path() );
		    }
		    else
		    {
			strcpy( buff, "file:" );
			strcat( buff, u.path() );
			if ( buff[ strlen( buff ) - 1 ] != '/' )
			    strcat( buff, "/" );
			strcat( buff, de->name );
		    }
		    
		    struct stat sbuff;
		    stat( buff+5, &sbuff );

		    struct stat lbuff;
		    lstat( buff+5, &lbuff );

		    // Test wether the file matches our filters
		    if ( ( filter & QDir::Dirs ) == QDir::Dirs && S_ISDIR( sbuff.st_mode ) )
			ok = TRUE;
		    if ( ( filter & QDir::Files ) == QDir::Files && S_ISREG( sbuff.st_mode ) )
			ok = TRUE;
		    if ( ( filter & QDir::Drives ) == QDir::Drives && 
			 ( S_ISCHR( sbuff.st_mode ) || S_ISBLK( sbuff.st_mode ) ) )
			ok = TRUE;
		    if ( ( filter & QDir::Executable ) == QDir::Executable &&
			 ( sbuff.st_mode & ( S_IXUSR | S_IXGRP | S_IXOTH ) != 0 ) &&
			 !S_ISDIR( sbuff.st_mode ) )
			ok = TRUE;
		    if ( ( filter & QDir::Hidden ) == QDir::Hidden && de->name[0] == '.' &&
			 de->name != "../" )
			ok = TRUE;
		    else if ( filter == 0 )
			ok = TRUE;
		 
		    if ( pname != 0L )
		    {
			if ( re.match( de->name ) == -1 )
			    ok = FALSE;
		    }
		    
		    if ( ok && ok2 )
		    {
			// Remember all files we already displayed
			//if ( !rest )
			//{
			QString *s = new QString( de->name );
			displayedFiles.append( s );
			//}
			
			QString e( "<cell><a href=\"" );
			e += buff;
			e += "\"><center>";
			_ipc->data( IPCMemory( e, e.length() ) );
			// KFM has to substitue this by the correct icon
			// This tag must be submitted in an extra data block!
			e = "<icon ";
			e += buff;
			e += ">";
			_ipc->data( IPCMemory( e, e.length() ) );
			e = "<br>";
			// Is it a link
			if ( de->access[0] == 'l' )
			{
			    e += "<i>";
			    e += de->name;
			    e += "</i>";
			}
			else
			    e += de->name;
			e += "</center><br></a></cell>";
			_ipc->data( IPCMemory( e, e.length() ) );
		    }
		}
	    }
	}
	
	old = ++p;	
    }
    
    _ipc->data( IPCMemory( old, strlen( old ) ) );

    free( mem );

    return SUCCESS;
}

#include "file.moc"
