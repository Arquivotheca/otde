 /*
  This file is or will be part of KDE desktop environment

  Copyright 1998 Sven Radej <sven@lisa.exp.univie.ac.at>

  It is licensed under GPL version 2.
  
  If it is part of KDE libraries than this file is licensed under
  LGPL version 2.
 */

#include "kregistry.h"
#include "kdirwatch.h"

#include <string.h>
#include <string>

#include <qdir.h>
#include <kapp.h>

#include <iostream.h>

/*******************************************************
 *
 * KRegistry
 *
 *******************************************************/

KRegistry *KRegistry::m_pSelf = 0L;

KRegistry::KRegistry()
{
  m_pSelf = this;

  m_lstEntries.setAutoDelete( true );

  m_pDirWatch = new KDirWatch;
 
  connect( m_pDirWatch, SIGNAL(dirty(const char *)),
	   this, SLOT(update(const char *)));
  connect( m_pDirWatch, SIGNAL(deleted(const char *)),
	   this, SLOT(dirDeleted(const char *))); 
}

KRegistry::~KRegistry()
{
}

void KRegistry::addFactory( KRegFactory *_factory )
{
  m_lstFactories.append( _factory );

  const char *s;
  for( s = _factory->pathList().first(); s != 0L; s = _factory->pathList().next() )
  {    
    m_lstToplevelDirs.append( s );
    readDirectory( s, m_lstEntries, true );
  }
}

bool KRegistry::readDirectory( const char* _path, QList<KRegEntry>& l, bool _init )
{
  cerr << "Reading directory " << _path << endl;
  
  QDir d( _path );                               // set QDir ...
  if ( !d.exists() )                            // exists&isdir?
    return false;                             // return false
  d.setSorting(QDir::Name);                  // just name

  QString path( _path );
  if ( path.right(1) != "/" )
    path += "/";
 
  QString file;

  //************************************************************************
  //                           Mark
  //************************************************************************
 
  QListIterator<KRegEntry> it( l );
  for( ; it.current(); ++it )
    it.current()->unmark();
   
  //************************************************************************
  //                           Setting dirs
  //************************************************************************
 
  if ( _init )                                  // first time?
    m_pDirWatch->addDir(path);          // add watch on this dir
  
  // Note: If some directory is gone, dirwatch will delete it from the list.
  
  //************************************************************************
  //                               Reading
  //************************************************************************
 
 unsigned int i;                           // counter and string length.
 unsigned int count = d.count();
 for( i = 0; i < count; i++ )                        // check all entries
 {
   if (strcmp(d[i],".")==0 || strcmp(d[i],"..")==0 || strcmp(d[i],"magic")==0)
    continue;                          // discard those ".", "..", "magic"...

   file = path.data();                          // set full path
   file += d[i];                          // and add the file name.
   if ( stat( file, &m_statbuff ) == -1 )           // get stat...
    continue;                                   // no such, continue.

   if ( S_ISDIR( m_statbuff.st_mode ) )               // isdir?
   {
     if ( _init )                                 // first time?
       readDirectory( file, l, _init );      // yes, dive into it.
     else if ( !m_pDirWatch->contains( file ) ) // New dir?
     {
       readDirectory( file, l, _init );      // yes, dive into it.
       m_pDirWatch->addDir( file );         // and add it to the list
     }
   }
   else                                         // no, not a dir/no recurse...
   {
     int i2 = -1;                                    // index
     if ( !_init )                                 // first time?
       i2 = ( exists( file, l ) );                  // find it in list...
     if ( i2 != -1 )                              // got it?
     {                                         // Yeah!
       KRegEntry *entry = l.at( i2 );
       entry->mark();
       KRegEntry *e = entry->update();                // update it (if needed)
       // Replace ?
       if ( e )
       {
	 l.insert( i2, e );
	 l.removeRef( entry );
       }
     }
     else if ( file.right(1) != "~" )         // we don't have this one..
     {
       // Can we read the file ?
       if ( access( file, R_OK ) != -1 )
       {   
	 // Create a new entry
	 KRegEntry *entry = createEntry( file );
	 l.append( entry );
	 if ( !_init )
	   cerr << "KRegistry: New item " << file << endl;
       }
     }
   }                                        // that's all
 }

 // Now: what if file was removed from dir? We parsed only those found by
 // QDir, but we should kick out from the list deleted ones. This is what we
 // do:
 // We loop thro' the list to check if all items exist. And we remove those
 // that do not exist. Since this is recursive function, we use flag
 // 'inner'. If we're called recursively this flag is true.

 if ( _init )    // pointless if not top level or if first time
   return true;

 //************************************************************************
 //                        Removing deleted from list
 //                                  Sweep
 //************************************************************************

 KRegEntry *a = l.first();
 while( a )
 {
   if ( !a->isMarked() && a->isInDirectory( path ) )
   {
     debug ("KRegistry: Deleted item %s", a->file());
     l.remove( l.at() );
     a = l.current();
     continue;
   }
   a = l.next();
 }

 return true;
}

int KRegistry::exists( const char *_file, QList<KRegEntry>& _list)
{
  int pos = 0;
  QListIterator<KRegEntry> it( _list );
  for( ; it.current(); ++it )
  {
    if ( strcmp( it.current()->file(), _file ) == 0 )
      return pos;
    pos++;
  }

  return -1;
}

bool KRegistry::load()
{
  return false;
}

bool KRegistry::save()
{
  return false;
}

void KRegistry::update( const char *_path )
{
  if ( _path )
  {
    cerr << "Update for " << _path << endl;
    readDirectory( _path, m_lstEntries );
  }
  else
  {
    const char *s;
    for( s = m_lstToplevelDirs.first(); s != 0L; s = m_lstToplevelDirs.next() )
      readDirectory( s, m_lstEntries );
  }
}

void KRegistry::dirDeleted( const char *_path )
{
  cerr << "KRegistry: Dir deleted " << _path << endl;
  
  KRegEntry *a = m_lstEntries.first();
  while( a )
  {
    if ( a->isInDirectory( _path, true ) )
    {
      cerr << "KRegistry: Deleted item " << a->file() << endl;
      m_lstEntries.remove( m_lstEntries.at() );
      a = m_lstEntries.current();
      continue;
    }
    a = m_lstEntries.next();
  }
}

KRegEntry* KRegistry::createEntry( const char *_file )
{
  QString file = _file;
  // Just a backup file ?
  if ( file.right(1) == "~" || file.right(4) == ".bak" || ( file[0] == '%' && file.right(1) == "%" ) )
      return new KRegDummy( this, _file );

  KSimpleConfig cfg( _file, true );
  cfg.setGroup( "KDE Desktop Entry" );
  QString type = cfg.readEntry( "Type" );
  
  KRegFactory *f;
  for( f = m_lstFactories.first(); f != 0L; f = m_lstFactories.next() )
  {
    if ( type == f->type() && f->matchFile( _file ) )
    {
      KRegEntry *e = f->create( this, _file, cfg );
      if ( e )
	return e;
      break;
    }
  }
  
  return new KRegDummy( this, _file );
}

/***************************************************
 *
 * KRegEntry
 *
 ***************************************************/

KRegEntry::KRegEntry( KRegistry* _reg, const char* _file )
{
  m_bMarked = true;
  m_strFile = _file;
  m_pRegistry = _reg;
  
  struct stat statbuff;
  if ( stat( m_strFile, &statbuff ) == -1 )
  {
    cerr << "Oooops " << m_strFile << endl;
  }

  m_ctime = statbuff.st_ctime;
}

bool KRegEntry::isInDirectory( const char *_path, bool _allow_subdir )
{
  if ( _allow_subdir )
  {
    if ( strncmp( m_strFile.data(), _path, strlen( _path ) ) == 0 )
      return true;
    return false;
  }
  
  unsigned int pos = m_strFile.findRev( '/' );
  if ( pos != strlen( _path ) )
    return false;
  
  if ( strncmp( _path, m_strFile.data(), pos + 1 ) == 0 )
    return true;
  
  return false;
}

KRegEntry* KRegEntry::update()
{
  cerr << "Checking " << m_strFile << endl;

  struct stat statbuff;
  if (stat( m_strFile, &statbuff) == -1)
  {
    cerr << "Removing us" << endl;
    // We are going to be deleted now
    unmark();
    return 0L;
  }

  if ( access( m_strFile, R_OK ) == -1 )
  {
    cerr << "We are no longer readable" << endl;
    unmark();
    return 0L;
  }
 
  if ( statbuff.st_ctime == m_ctime )
    return 0L; // nothing happened 

  if ( !updateIntern() )
    return m_pRegistry->createEntry( m_strFile );
    
  // Means that we indeed updated
  return 0L;
}

/*******************************************************
 *
 * KRegFactory
 *
 *******************************************************/

bool KRegFactory::matchFile( const char *_file )
{
  QString file = _file;
  int i = file.findRev( '/' );
  if ( i == -1 )
    return false;
  file = file.left( i + 1 );
  
  const char *s;
  for( s = pathList().first(); s != 0L; s = pathList().next() )
  {    
    QString p = s;
    if ( p.right(1) != "/" )
      p += "/";
  
    if ( strncmp( p.data(), file.data(), p.length() ) == 0 && p.length() <= file.length() )
      return true;
  }
  
  return false;
}

#include "kregistry.moc"
