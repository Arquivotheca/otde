// Popup menus for kfm icons. Only the 'New' submenu for the moment.
// (c) David Faure, 1998

#include <qdir.h>
#include <qmsgbox.h>

#include <kapp.h>
#include <ksimpleconfig.h>

#include "kfmpaths.h"
#include "kfmdlg.h"
#include "kiojob.h"
#include "popup.h"

QStrList * KNewMenu::templatesList = 0L;
int KNewMenu::templatesVersion = 0;

KNewMenu::KNewMenu() : QPopupMenu(), menuItemsVersion(0)
{
    fillMenu();
    connect( this /* as a menu */, SIGNAL( activated( int ) ), 
	     this /* as a receiver */, SLOT( slotNewFile( int ) ) );
    connect( this /* as a menu */, SIGNAL(aboutToShow()), 
             this /* as a receiver */, SLOT( slotCheckUpToDate() ));
}

void KNewMenu::slotCheckUpToDate( )
{
    if (menuItemsVersion < templatesVersion)
    {
        fillMenu();
        menuItemsVersion = templatesVersion;
    }
}

void KNewMenu::fillMenu()
{
    if (!templatesList) { // No templates list up to now
        templatesList = new QStrList();
        fillTemplates();
        menuItemsVersion = templatesVersion;
    }

    this->clear();
    this->insertItem( klocale->translate("Folder") );

    char * templ = templatesList->first(); // skip 'Folder'
    for ( templ = templatesList->next(); (templ); templ = templatesList->next())
    {
        QString tmp = templ;
        KSimpleConfig config(KFMPaths::TemplatesPath() + tmp.data());
        config.setGroup( "KDE Desktop Entry" );
        if ( tmp.right(7) == ".kdelnk" )
            tmp.truncate( tmp.length() - 7 );
        this->insertItem( config.readEntry("Comment", tmp ) );
    }
}

void KNewMenu::fillTemplates()
{
    templatesVersion++;

    templatesList->clear();    
    templatesList->append( QString( "Folder") );

    QDir d( KFMPaths::TemplatesPath() );
    const QFileInfoList *list = d.entryInfoList();
    if ( list == 0L )
        warning(klocale->translate("ERROR: Template does not exist '%s'"),
		KFMPaths::TemplatesPath().data());
    else
    {
	QFileInfoListIterator it( *list );      // create list iterator
	QFileInfo *fi;                          // pointer for traversing

	while ( ( fi = it.current() ) != 0L )
	{
	    if ( strcmp( fi->fileName().data(), "." ) != 0 && 
		 strcmp( fi->fileName().data(), ".." ) != 0 )
	    {
		QString tmp = fi->fileName().data();
		templatesList->append( tmp );
	    }
	    ++it;                               // goto next list element
	}
    }
}

void KNewMenu::slotNewFile( int _id )
{
    if ( this->text( _id ) == 0)
	return;

    QString p = templatesList->at( _id );
    QString tmp = p;
    tmp.detach();

    if ( strcmp( tmp.data(), "Folder" ) != 0 ) {
      QString x = KFMPaths::TemplatesPath() + p.data();
      if (!QFile::exists(x.data())) {
          QMessageBox::message( i18n( "KFM Error" ), i18n(
              "Source file doesn't exist anymore ! \n"  
              "Use \"Rescan Bindings\" in View menu to update the menu"));
          return;
      }
      KSimpleConfig config(x);
      config.setGroup( "KDE Desktop Entry" );
      if ( tmp.right(7) == ".kdelnk" )
	tmp.truncate( tmp.length() - 7 );
      tmp = config.readEntry("Comment", tmp);
    }
    
    QString text = klocale->translate("New ");
    text += tmp.data();
    text += ":";
    const char *value = p.data();

    if ( strcmp( tmp.data(), "Folder" ) == 0 ) {
	value = "";
	text = klocale->translate("New ");
	text += klocale->translate("Folder");
	text += ":";
    }
    
    DlgLineEntry l( text.data(), value, 0L /*view->getGUI()*/ );
    if ( l.exec() )
    {
	QString name = l.getText();
	if ( name.length() == 0 )
	    return;
	
        QStrList urls = popupFiles;
        char *s;
	if ( strcmp( p.data(), "Folder" ) == 0 )
	{
            for ( s = urls.first(); s != 0L; s = urls.next() )
	    {
     	      KIOJob * job = new KIOJob;
              QString u = s;
              u.detach();
	      if ( u.right( 1 ) != "/" )
		u += "/";
              u += name.data();
	      job->mkdir( u.data() );
            }
	}
	else
	{
	    QString src = KFMPaths::TemplatesPath() + p.data();
            for ( s = urls.first(); s != 0L; s = urls.next() )
            {
                KIOJob * job = new KIOJob;
                QString dest = s;
                dest.detach();
                if ( dest.right( 1 ) != "/" )
                    dest += "/";
                dest += name.data();
                // debugT("Command copy '%s' '%s'\n",src.data(),dest.data());
                job->copy( src.data(), dest.data() );
            }
	}
    }
}

#include "popup.moc"
