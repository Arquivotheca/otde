#include "kregfactories.h"
#include "kmimetypes.h"
#include "kservices.h"

#include <qstring.h>
#include <qmsgbox.h>

#include <kapp.h>

/**************************************************
 *
 * KMimeTypeEntry
 *
 **************************************************/

KMimeTypeEntry::KMimeTypeEntry( KRegistry* _reg, const char *_file, KMimeType *_mime )
  : KRegEntry( _reg, _file )
{
  m_pMimeType = _mime;
}

KMimeTypeEntry::~KMimeTypeEntry()
{
  if ( m_pMimeType )
    delete m_pMimeType;
}

bool KMimeTypeEntry::updateIntern()
{
  if ( m_pMimeType )
    delete m_pMimeType;
  m_pMimeType = 0L;
  
  return false;
}

/**************************************************
 *
 * KMimeTypeFactory
 *
 **************************************************/

KMimeTypeFactory::KMimeTypeFactory()
{
  m_lst.append( kapp->kde_mimedir() );
  QString tmp = kapp->localkdedir();
  tmp += "/share/mimelnk";
  m_lst.append( tmp );
}
 
KRegEntry* KMimeTypeFactory::create( KRegistry* _reg, const char *_file, KSimpleConfig &_cfg )
{
  KMimeType *mime = createMimeType( _file, _cfg );
  if ( !mime )
    return 0L;
  
  return new KMimeTypeEntry( _reg, _file, mime );
}

KMimeType* KMimeTypeFactory::createMimeType( const char *_file, KSimpleConfig &_cfg )
{
  cerr << "Parsing " << _file << endl;
  
  // Get a ';' separated list of all pattern
  QString pats = _cfg.readEntry( "Patterns" );
  QString icon = _cfg.readEntry( "Icon" );
  QString comment = _cfg.readEntry( "Comment" );
  QString mime = _cfg.readEntry( "MimeType" );

  if ( mime.isEmpty() )
  {
    string tmp = i18n( "The mime type _cfg file " );
    tmp += _file;
    tmp += i18n("does not conatain a MimeType=... entry" );
    QMessageBox::critical( 0L, i18n( "KFM Error" ), tmp.c_str(), i18n( "Ok" ) );
    return 0L;
  }
      
  // Is this file type already registered ?
  // TODO
  /* if ( KMimeType::find( mime ) )
    continue; */

  // If not then create a new type
  if ( icon.isEmpty() )
    icon = "unknown.xpm";
      
  QStrList patterns;
  int pos2 = 0;
  int old_pos2 = 0;
  while ( ( pos2 = pats.find( ";", pos2 ) ) != - 1 )
  {
    // Read a pattern from the list
    QString name = pats.mid( old_pos2, pos2 - old_pos2 );
    patterns.append( name );
    pos2++;
    old_pos2 = pos2;
  }

  KMimeType *e;
  
  if ( mime == "inode/directory" )
    e = new KFolderType( mime.data(), icon.data(), comment.data(), patterns );
  else if ( mime == "application/x-kdelnk" )
    e = new KDELnkMimeType( mime.data(), icon.data(), comment.data(), patterns );
  else if ( mime == "application/x-executable" || mime == "application/x-shellscript" )
    e = new KExecMimeType( mime.data(), icon.data(), comment.data(), patterns );
  else
    e = new KMimeType( mime.data(), icon.data(), comment.data(), patterns );

  return e;
}

const char* KMimeTypeFactory::type()
{
  return "MimeType";
}

QStrList& KMimeTypeFactory::pathList()
{
  return m_lst;
}

/**************************************************
 *
 * KServiceEntry
 *
 **************************************************/

KServiceEntry::KServiceEntry( KRegistry* _reg, const char *_file, KService *_service )
  : KRegEntry( _reg, _file )
{
  m_pService = _service;
}

KServiceEntry::~KServiceEntry()
{
  if ( m_pService )
    delete m_pService;
}

bool KServiceEntry::updateIntern()
{
  if ( m_pService )
    delete m_pService;
  m_pService = 0L;
  
  return false;
}

/**************************************************
 *
 * KServiceFactory
 *
 **************************************************/

KServiceFactory::KServiceFactory()
{
  m_lst.append( kapp->kde_appsdir() );
  QString tmp = kapp->localkdedir();
  tmp += "/share/applnk";
  m_lst.append( tmp );
}
 
KRegEntry* KServiceFactory::create( KRegistry* _reg, const char *_file, KSimpleConfig &_cfg )
{
  KService *service = createService( _file, _cfg );
  if ( !service )
    return 0L;
  
  return new KServiceEntry( _reg, _file, service );
}

KService* KServiceFactory::createService( const char *_file, KSimpleConfig &_cfg )
{
  cerr << "Parsing " << _file << endl;
  
  return KService::parseService( _file, _cfg );
}

const char* KServiceFactory::type()
{
  return "Application";
}

QStrList& KServiceFactory::pathList()
{
  return m_lst;
}
