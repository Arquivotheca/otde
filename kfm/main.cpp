#include "kbind.h"
#include "kfmgui.h"
#include "root.h"
#include "kfmserver.h"
#include "xview.h"
#include "kfmpaths.h"
#include <config-kfm.h>

#include <kapp.h>
#include <unistd.h>
#include <html.h>
#include <qmsgbox.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include <sys/types.h>
#include <dirent.h>
#include <signal.h>
#include <sys/wait.h>

void autostart();
void testDir();
void sig_handler( int signum );

#include <klocale.h>
static KLocale locale("kfm");

void testDir( const char *_name )
{
  DIR *dp;
  dp = opendir( _name );
  if ( dp == NULL )
  {
    QString m = _name;
    QMessageBox::message( locale.translate("KFM Information"), 
			  locale.translate("Creating directory:\n") + m );
    ::mkdir( _name, S_IRWXU );
  }
  else
    closedir( dp );
}

int main( int argc, char ** argv )
{    
    // Test for config file
    QString c = getenv( "HOME" );
    c += "/.kde";
    DIR *dp;
    dp = opendir( c.data() );
    if ( dp == NULL )
	::mkdir( c.data(), S_IRWXU );
    else
    {
	debugT("Exist '%s'\n", c.data() );
	closedir( dp );
    }
    
    c = getenv( "HOME" );
    c += "/.kde/config";
    dp = opendir( c.data() );
    if ( dp == NULL )
	::mkdir( c.data(), S_IRWXU );
    else
    {
	debugT("Exist '%s'\n", c.data() );
	closedir( dp );
    }
    
    c = getenv( "HOME" );
    c += "/.kde/config/kfmrc";
    FILE *f2 = fopen( c.data(), "rb" );
    if ( f2 == 0L )
    {
	QString cmd;
	cmd.sprintf( "cp %s/lib/kfm/config/kfmrc %s/.kde/config/kfmrc", kapp->kdedir().data(), getenv( "HOME" ) );
	//	system( cmd.data() );
    }
    else
	fclose( f2 );

    // Test for kfm directories
    c = getenv( "HOME" );
    c += "/.kfm";
    dp = opendir( c.data() );
    if ( dp == NULL )
	::mkdir( c.data(), S_IRWXU );
    else
    {
	debugT("Exist '%s'\n", c.data() );
	closedir( dp );
    }
    
    c = getenv( "HOME" );
    c += "/.kfm/cache";
    dp = opendir( c.data() );
    if ( dp == NULL )
	::mkdir( c.data(), S_IRWXU );
    else
    {
	debugT("Exist '%s'\n", c.data() );
	closedir( dp );
    }

    c = getenv( "HOME" );
    c += "/.kfm/tmp";
    dp = opendir( c.data() );
    if ( dp == NULL )
	::mkdir( c.data(), S_IRWXU );
    else
    {
	debugT("Exist '%s'\n", c.data() );
	closedir( dp );
    }

    c = getenv( "HOME" );
    c += "/.desktop";
    f2 = fopen( c.data(), "rb" );
    if ( f2 == 0L )
    {
	QString cmd;
	cmd.sprintf( "cp %s/lib/kfm/config/desktop %s/.desktop", kapp->kdedir().data(), getenv( "HOME" ) );
	system( cmd.data() );
    }
    else
	fclose( f2 );

    KApplication a( argc, argv, "kfm" );
    
    // Stephan: I must find a better place for this somewhen
    KFMPaths::initPaths();
    
    // Test for existing Templates
    bool bTemplates = true;
    
    dp = opendir( KFMPaths::TemplatesPath().data() );
    if ( dp == NULL )
	bTemplates = false;
    else
      closedir( dp );

    signal(SIGCHLD,sig_handler);

    // Test for directories
    QString d;

    testDir( KFMPaths::DesktopPath() );
    testDir( KFMPaths::TrashPath() );
    testDir( KFMPaths::TemplatesPath() );
    testDir( KFMPaths::AutostartPath() );

    QString kd = kapp->kdedir();
    d = kd.copy();
    d += "/apps";
    testDir( d );
    d = kd.copy();
    d += "/mimetypes";
    testDir( d );
    d = kd.copy();

    // Stephan: This must change in the near future
    d += "/lib/pics";
    testDir( d );
    d = kd.copy();
    d += "/lib/pics/toolbar";
    testDir( d );
    d = kd.copy();
    d += "/lib/pics/wallpapers";
    testDir( d );

    if ( !bTemplates )
    {
	QMessageBox::message( locale.translate("KFM Information"),
			      locale.translate("Installing Templates") );
	QString cmd;
	cmd.sprintf("cp %s/lib/kfm/Desktop/Templates/* %s", 
		    kapp->kdedir().data(), KFMPaths::TemplatesPath().data() );
	system( cmd.data() );
    }
    
    KHTMLWidget::registerFormats();
    QImageIO::defineIOHandler( "XV", "^P7 332", 0, read_xv_file, 0L );
    
    debugT("0. Init IPC\n");
    
    KFMServer ipc;
    
    QString file = QDir::homeDirPath();
    file += "/.kfm.run";
    
    FILE *f = fopen( file.data(), "wb" );
    if ( f == 0L )
    {
	debugT("ERROR: Could not write PID file\n");
	exit(1);
    }
    fprintf( f, "%i\n%i\n", (int)getpid(),(int)ipc.getPort() );
    fclose( f );

    // Stephan: alias some translated string to find them faster
    locale.aliasLocale("Open", ID_STRING_OPEN);
    locale.aliasLocale("Cd", ID_STRING_CD);
    locale.aliasLocale("New View", ID_STRING_NEW_VIEW);
    locale.aliasLocale("Copy", ID_STRING_COPY);
    locale.aliasLocale("Delete", ID_STRING_DELETE);
    locale.aliasLocale("Move to Trash", ID_STRING_MOVE_TO_TRASH);
    locale.aliasLocale("Paste", ID_STRING_PASTE);
    locale.aliasLocale("Open with", ID_STRING_OPEN_WITH);
    locale.aliasLocale("Cut", ID_STRING_CUT);
    locale.aliasLocale("Move", ID_STRING_MOVE);
    locale.aliasLocale("Properties", ID_STRING_PROP);
    locale.aliasLocale("Link", ID_STRING_LINK);
    locale.aliasLocale("Empty Trash Bin", ID_STRING_TRASH);

    debugT("1. Init KIOManager\n");

    //Stephan: This variable is not deleted here, but in the 
    // slotQuit methode of KFileWindow. I'm not that sure, if
    // this is the best way! 
    // KIOServer *server = new KIOServer();
    new KIOServer();

    debugT("2. Init FileTypes\n");
    
    KMimeType::init();
    
    debugT("3. Init Root widget\n");

    new KRootWidget();
    
    debugT("4. Init window\n");
    
    bool openwin = true;

    int arg = 1;
    
    if ( argc > arg )
	if ( strcmp( argv[arg++], "-d" ) == 0 )
	    openwin = false;
    
    if ( openwin )
    {
	QString home = "file:";
	home.detach();
	home += QDir::homeDirPath().data();
	debugT("Opening window\n");
	KfmGui *m = new KfmGui( 0L, 0L, home.data() );
	m->show();
	debugT("Opended\n");
    }
    
    QWidget w( 0L, "Main" );
    a.setMainWidget( &w );

    Window root = DefaultRootWindow( a.getDisplay() );
    Window win = w.winId();
    Atom atom = XInternAtom( a.getDisplay(), "DndRootWindow", False );    
    XChangeProperty( a.getDisplay(), root, atom, XA_STRING, 32,
		     PropModeReplace, (const unsigned char*)(&win), 1);
    debugT("Root window = %x\n",(int)win);

	debugT("Fetching RootWindow\n");
	
	unsigned char *Data;
	unsigned long Size;
        Atom    ActualType;
        int     ActualFormat;
        unsigned long RemainingBytes;
      
	debugT("Call\n");

        XGetWindowProperty(a.getDisplay(),root,atom,
                           0L,4L,
                           false,AnyPropertyType,
                           &ActualType,&ActualFormat,
                           &Size,&RemainingBytes,
                           &Data);

	debugT("Called and Data is %x\n",*Data);

	if ( Data != 0L )
	    win = *((Window*)Data);
	else
	    win = 0L;
	
	debugT("root window is %x\n",(int)win);

    debugT("5. running\n");

    bool as = true;
    
    if ( argc > arg )
	if ( strcmp( argv[arg++], "-na" ) == 0 )    
	  as = false;
    
    if ( as )
      autostart();

    debugT("OOOOOOOOOOOOOOOOOO Display size %i %i\n",
	   XDisplayWidth( a.getDisplay(), 0 ), XDisplayHeight( a.getDisplay(), 0 ) );
    
    return a.exec();
}

void sig_handler( int )
{
    int pid;
    int status;
    
    while( 1 )
    {
	debugT("SIGNAL HANDLER called\n");
	
	pid = waitpid( -1, &status, WNOHANG );
	if ( pid <= 0 )
	{
	    // Reinstall signal handler, since Linux resets to default after
	    // the signal occured ( BSD handles it different, but it should do
	    // no harm ).
	    signal(SIGCHLD,sig_handler);
	    return;
	}
    }
}
