#include <qfile.h>
#include <qdir.h>
#include <qdialog.h>
#include <qpixmap.h>

#include <kapp.h>
#include <ksimpleconfig.h>
#include <ktreelist.h>
#include <ktopwidget.h>
#include <kiconloader.h>
#include <kservices.h>

#include <string.h>
#include <stdio.h>

#include "kio_openwith.moc"

#define SORT_SPEC (QDir::DirsFirst | QDir::Name | QDir::IgnoreCase)

// ----------------------------------------------------------------------

KAppTreeListItem::KAppTreeListItem( const char *name, QPixmap *pixmap, 
     bool d, bool parse, bool dir, QString p, QString c )
    : KTreeListItem( name, pixmap )
{
  dummy = d;
  parsed = parse;
  directory = dir;
  path = p;
  c.simplifyWhiteSpace();
  int pos;
  if( (pos = c.find( ' ')) > 0 )
    exec = c.left( pos );
  else
    exec = c;
  appname = name;
}

// ----------------------------------------------------------------------

KApplicationTree::KApplicationTree( QWidget *parent ) : QWidget( parent )
{
  tree = new KTreeList( this );
  tree->setSmoothScrolling( true );
  setFocusProxy( tree );
  
  QString personal = KApplication::localkdedir().data();
  personal += "/share/applnk";
  QString global   = KApplication::kde_appsdir().data();
  
  parseKdelnkDir( QDir(personal), tree );
  parseKdelnkDir( QDir(global), tree );
  
  tree->show();
  connect( tree, SIGNAL( expanded(int) ), SLOT( expanded(int) ) );
  connect( tree, SIGNAL( highlighted(int) ), SLOT( highlighted(int) ) );
  connect( tree, SIGNAL( selected(int) ), SLOT( selected(int) ) );
}

bool KApplicationTree::isKdelnkFile( const char* filename )
{
  FILE *f;
  f = fopen( filename, "rb" );
  if ( f == 0L )
    return false;

  char buff[ 100 ];
  buff[ 0 ] = 0;
  fgets( buff, 100, f );
  fclose( f );
	  
  if ( strncmp( buff, "# KDE Config File", 17 ) != 0L )
    return false;

  return true;
}

void KApplicationTree::parseKdelnkFile( QFileInfo *fi, KTreeList *tree, KAppTreeListItem *item )
{
  QPixmap pixmap;
  QString text_name, pixmap_name, mini_pixmap_name, big_pixmap_name, command_name, comment;

  QString file = fi->absFilePath();

  if( fi->isDir() ) 
  {
    text_name = fi->fileName();
    file += "/.directory";
  }
  else
  {
    int pos = fi->fileName().find( ".kdelnk" );
    if( pos >= 0 )
      text_name = fi->fileName().left( pos );
    else
      text_name = fi->fileName();
  }

  QFile config( file );

  if( config.exists() )
  {
    KSimpleConfig kconfig( file, true );
    kconfig.setGroup("KDE Desktop Entry");
    command_name      = kconfig.readEntry("Exec");
    mini_pixmap_name  = kconfig.readEntry("MiniIcon");
    big_pixmap_name   = kconfig.readEntry("Icon");
    comment           = kconfig.readEntry("Comment");
    text_name         = kconfig.readEntry("Name", text_name);

    if( !mini_pixmap_name.isEmpty() )
      pixmap = KApplication::getKApplication()->getIconLoader()->loadApplicationMiniIcon(mini_pixmap_name, 16, 16);
    if( pixmap.isNull() && !big_pixmap_name.isEmpty() )
      pixmap = KApplication::getKApplication()->getIconLoader()->loadApplicationMiniIcon(big_pixmap_name, 16, 16);
    if( pixmap.isNull() )
      pixmap = KApplication::getKApplication()->getIconLoader()->loadApplicationMiniIcon("mini-default.xpm", 16, 16);	
  }
  else
  {
     command_name = text_name;
     pixmap = KApplication::getKApplication()->getIconLoader()->loadApplicationMiniIcon("mini-default.xpm", 16, 16); 
  }

  it2 = new KAppTreeListItem( text_name.data(), &pixmap, false, false, fi->isDir(), fi->absFilePath(), command_name );	

    if( item == 0 )
      tree->insertItem( it2 );
    else
      item->appendChild( it2 );

  if( fi->isDir() )
  {
    dummy = new KAppTreeListItem( i18n("This group is empty!"), 0, true, false, false, "", "" );	
    it2->appendChild( dummy );
  }
}

short KApplicationTree::parseKdelnkDir( QDir d, KTreeList *tree, KAppTreeListItem *item = 0)
{
  if( !d.exists() )
    return -1;

  d.setSorting( SORT_SPEC );
  QList <QString> item_list;
	
  const QFileInfoList *list = d.entryInfoList();
  QFileInfoListIterator it( *list );
  QFileInfo *fi;

  if( it.count() < 3 )
    return -1;

  while( ( fi = it.current() ) )
  {
    if( fi->fileName() == "." || fi->fileName() == ".." )
    {
      ++it;
      continue;
    }
    if( fi->isDir() )
    {
      parseKdelnkFile( fi, tree, item );
    }
    else 
    {
      if( !isKdelnkFile( fi->absFilePath() ) )
      {
	++it;
	continue;
      }
      parseKdelnkFile( fi, tree, item );
    } 
    ++it;
  }
  
  return 0;
}

void KApplicationTree::expanded(int index)
{
  KAppTreeListItem *item = (KAppTreeListItem *)tree->itemAt( index );

  if( !item->parsed && !(item->dummy ) )
  {
     parseKdelnkDir( QDir(item->path), tree, item );
     item->parsed = true;
     if( item->childCount() > 1 )
        item->removeChild( item->getChild() );
  }
}


void KApplicationTree::highlighted(int index)
{

  KAppTreeListItem *item = (KAppTreeListItem *)tree->itemAt( index );

  if( ( !item->directory ) && 
      !( item->exec.isEmpty() ) && 
      !( item->appname.isEmpty() ) )

    emit highlighted( item->appname.data(), item->exec.data() ); 

}


void KApplicationTree::selected(int index)
{
  KAppTreeListItem *item = (KAppTreeListItem *)tree->itemAt( index );

  if( ( !item->directory ) && !( item->exec.isEmpty() ) && !( item->appname.isEmpty() ) )
     emit selected( item->appname.data(), item->exec.data() ); 
  else
    tree->expandOrCollapseItem( index );
}

void KApplicationTree::resizeEvent( QResizeEvent * )
{
  tree->setGeometry( 0, 0, width(), height() );
}

/***************************************************************
 *
 * OpenWithDlg
 *
 ***************************************************************/

OpenWithDlg::OpenWithDlg( const char *_text, const char* _value, QWidget *parent, bool _file_mode )
        : QDialog( parent, 0L, true )
{
  m_pTree = 0L;
  m_pService = 0L;
  haveApp = false;
  
  setGeometry( x(), y(), 370, 100 );
  
  label = new QLabel( _text , this );
  label->setGeometry( 10, 10, 350, 15 );
  
  edit = new KLined( this, 0L );
    
  if ( _file_mode )
  {
    completion = new KURLCompletion();
    connect ( edit, SIGNAL (completion()),
	      completion, SLOT (make_completion()));
    connect ( edit, SIGNAL (rotation()),
	      completion, SLOT (make_rotation()));
    connect ( edit, SIGNAL (textChanged(const char *)),
	      completion, SLOT (edited(const char *)));
    connect ( completion, SIGNAL (setText (const char *)),
	      edit, SLOT (setText (const char *)));
  }
  else
    completion = 0L;

  edit->setGeometry( 10, 35, 350, 25 );
  connect( edit, SIGNAL(returnPressed()), SLOT(accept()) );
  
  ok = new QPushButton( klocale->translate("Ok"), this );
  ok->setGeometry( 10,70, 80,25 );
  connect( ok, SIGNAL(clicked()), SLOT(slotOK()) );
  
  browse = new QPushButton( klocale->translate("&Browser"), this );
  browse->setGeometry( 100, 70, 80, 25 );
  connect( browse, SIGNAL(clicked()), SLOT(slotBrowse()) );
  
  clear = new QPushButton( klocale->translate("Clear"), this );
  clear->setGeometry( 190, 70, 80, 25 );
  connect( clear, SIGNAL(clicked()), SLOT(slotClear()) );
  
  cancel = new QPushButton( klocale->translate("Cancel"), this );
  cancel->setGeometry( 280, 70, 80, 25 );
  connect( cancel, SIGNAL(clicked()), SLOT(reject()) );
  
  edit->setText( _value );
  edit->setFocus();
  haveApp = false;
}

OpenWithDlg::~OpenWithDlg()
{
  delete completion;
}

void OpenWithDlg::slotClear()
{
  edit->setText("");
}

void OpenWithDlg::slotBrowse()
{
  if ( m_pTree )
    return;
  
  browse->setEnabled( false );

  ok->setGeometry( 10,280, 80,25 );
  browse->setGeometry( 100, 280, 80, 25 );
  clear->setGeometry( 190, 280, 80, 25 );
  cancel->setGeometry( 280, 280, 80, 25 );

  m_pTree = new KApplicationTree( this );
  connect( m_pTree, SIGNAL( selected( const char*, const char* ) ), this, SLOT( slotSelected( const char*, const char* ) ) );

  connect( m_pTree, SIGNAL( highlighted( const char*, const char* ) ), this, SLOT( slotHighlighted( const char*, const char* ) ) );

  m_pTree->setGeometry( 10, 70, 350, 200 );
  m_pTree->show();
  m_pTree->setFocus();
  
  resize( width(), height() + 210 );
}

void OpenWithDlg::resizeEvent(QResizeEvent *)
{
  // someone will have to write proper geometry management 
  // but for now this will have to do ....

  if( m_pTree )
  {
    label->setGeometry( 10, 10, width() - 20, 15 );
    edit->setGeometry( 10, 35, width() - 20, 25 );
    ok->setGeometry( 10,height() - 30, (width()-20-30)/4,25 );
    browse->setGeometry( 10 + (width()-20-30)/4 + 10
			 ,height() - 30, (width()-20-30)/4, 25 );
    clear->setGeometry(10 + 2*((width()-20-30)/4) + 2*10 
		       ,height() - 30, (width()-20-30)/4, 25 );
    cancel->setGeometry( 10 + 3*((width()-20-30)/4) + 3*10 ,
			 height() - 30, (width()-20-30)/4, 25 );
    m_pTree->setGeometry( 10, 70, width() - 20, height() - 110 );

  }
  else
  {
    label->setGeometry( 10, 10, width() - 20, 15 );
    edit->setGeometry( 10, 35, width() - 20, 25 );
    ok->setGeometry( 10,height() - 30, (width()-20-30)/4,25);
    browse->setGeometry( 10 + (width()-20-30)/4 + 10,
			 height() - 30, (width()-20-30)/4, 25 );
    clear->setGeometry( 10 + 2*((width()-20-30)/4) + 2*10,
			height() - 30, (width()-20-30)/4, 25 );
    cancel->setGeometry( 10 + 3*((width()-20-30)/4) + 3*10,
			 height() - 30, (width()-20-30)/4, 25 );
  }
}

void OpenWithDlg::slotSelected( const char* _name, const char* _exec )
{
  m_pService = KService::findByName( _name );
  if ( !m_pService )
    edit->setText( _exec );
  else
    edit->setText( "" );

  accept();
}

void OpenWithDlg::slotHighlighted( const char* _name, const char* )
{
  qName = _name;
  qName.detach();
  haveApp = true;
}

void OpenWithDlg::slotOK()
{
  if( haveApp )
  {
    m_pService = KService::findByName( qName );
  }

  haveApp = false;
  accept();
}
