// -*- c++ -*-
/* This file is part of the KDE libraries
    Copyright (C) 1997, 1998 Richard Moore <rich@kde.org>
                  1998 Stephan Kulow <coolo@kde.org>
                  1998 Daniel Grana <grana@ie.iwi.unibe.ch>
    
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

// $Id$

#include "kfileinfo.h"

#include <sys/types.h>
#include <dirent.h>
#include <grp.h>
#include <sys/stat.h>
#include <pwd.h>
#include <unistd.h>

#include <qfileinf.h>
#include <qregexp.h>

#include <kapp.h>
#include <kfm.h>

KFileInfo::KFileInfo(const KDirEntry &e)
{
    myName= e.name;

    myName.replace( QRegExp("/$"), "" );
    mySize= e.size;
    myBaseURL= "";
    myDate= e.date;
    myOwner= e.owner;
    myGroup= e.group;
    myAccess= e.access;
    parsePermissions(e.access);
}

KFileInfo::KFileInfo(const QFileInfo &e)
{
    myName = e.fileName();
    mySize = e.size();
    myBaseURL = "";
    myDate= (e.lastModified()).toString();
    myOwner = e.owner();
    myGroup = e.group();
    myIsDir = e.isDir();
    myIsFile = e.isFile();
    myIsSymLink = e.isSymLink();
    myPermissions = 755;
}

KFileInfo::KFileInfo(const char *dir, const char *name)
{
    struct stat buf;
    myName = name;
    myBaseURL = dir;

    if (lstat(dir + myName, &buf) == 0) {
	myIsDir = (buf.st_mode & S_IFDIR) != 0;
        // check if this is a symlink to a directory
	if (S_ISLNK(buf.st_mode)) {
	  myIsSymLink = true;
	  struct stat st;
	  if (stat(dir + myName, &st) == 0) 
	      myIsDir = S_ISDIR(st.st_mode) != 0;
	  else
	      myName = ""; // indicate, that the link is broken
	} else
	    myIsSymLink = false;
	myDate = "unknown";
	mySize = buf.st_size;
	myIsFile = !myIsDir;
	struct passwd *pw = getpwuid(buf.st_uid);
	struct group * ge = getgrgid( buf.st_gid );
	if (pw)
	    myOwner = pw->pw_name;
	else
	    myOwner = i18n("unknown");
	if (ge)
	    myGroup = ge->gr_name;
	else
	    myGroup = i18n("unknown");
	myPermissions = buf.st_mode;
	parsePermissions(myPermissions);
	// myDate = buf.st_mtime;
	
    } else {
	// default
	debug("the file does not exist %s%s",dir, name);
	myName.insert(0, "?");
	mySize = 0;
	myIsFile = false;
	myIsDir = false;
	myPermissions = 0;
	parsePermissions(myPermissions);
    }
}

KFileInfo::~KFileInfo()
{
}

KFileInfo &KFileInfo::operator=(const KFileInfo &i)
{
    myName= i.myName;
    mySize= i.mySize;
    myBaseURL= i.myBaseURL;
    myDate= i.myDate;
    myOwner= i.myOwner;
    myGroup= i.myGroup;
    myIsDir= i.myIsDir;
    myPermissions= i.myPermissions;
    return *this;
}

bool KFileInfo::isReadable() const
{
    return (::access(myBaseURL + myName, R_OK | (myIsDir ? X_OK : 0)) == 0) ;
}

void KFileInfo::parsePermissions(const char *perms)
{
    myPermissions = 0;
    char p[11] = {0, 0, 0, 0, 0,
		  0, 0, 0, 0, 0, 
		  0};

    strncpy(p, perms, sizeof(p));
    
    myIsDir = (bool)(p[0] == 'd');
    myIsSymLink = (bool)(p[0] == 'l');
    myIsFile = !myIsDir;

    if(p[1] == 'r')
      myPermissions |= QFileInfo::ReadUser;

    if(p[2] == 'w')
      myPermissions |= QFileInfo::WriteUser;

    if(p[3] == 'x')
      myPermissions |= QFileInfo::ExeUser;

    if(p[4] == 'r')
      myPermissions |= QFileInfo::ReadGroup;

    if(p[5] == 'w')
      myPermissions |= QFileInfo::WriteGroup;

    if(p[6] == 'x')
      myPermissions |= QFileInfo::ExeGroup;

    if(p[7] == 'r')
      myPermissions |= QFileInfo::ReadOther;

    if(p[8] == 'w')
      myPermissions |= QFileInfo::WriteOther;

    if(p[9] == 'x')
      myPermissions |= QFileInfo::ExeOther;
}

void KFileInfo::parsePermissions(uint perm)
{
    char p[] = "----------";

    if (myIsDir)
	p[0]='d';
    else if (myIsSymLink)
	p[0]='l';

    if (perm & QFileInfo::ReadUser)
	p[1]='r';
    if (perm & QFileInfo::WriteUser)
	p[2]='w';
    if (perm & QFileInfo::ExeUser)
	p[3]='x';

    if (perm & QFileInfo::ReadGroup)
	p[4]='r';
    if (perm & QFileInfo::WriteGroup)
	p[5]='w';
    if (perm & QFileInfo::ExeGroup)
	p[6]='x';
    
    if (perm & QFileInfo::ReadOther)
	p[7]='r';
    if (perm & QFileInfo::WriteOther)
	p[8]='w';
    if (perm & QFileInfo::ExeOther)
	p[9]='x';

    myAccess = p;
}

