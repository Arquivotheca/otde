/*
 * theme.h
 *
 * Copyright (c) 1998 Stefan Taferner <taferner@kde.org> and
 *                    Roberto Alsina <ralsina@unl.edu.ar>
 *
 * Requires the Qt widget libraries, available at no cost at
 * http://www.troll.no/
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#ifndef THEME_H
#define THEME_H

#include <qstring.h>
#include <ksimpleconfig.h>
#include <qpixmap.h>
#include <qstrlist.h>

class KConfig;

#define ThemeInherited KSimpleConfig
class Theme: public KSimpleConfig
{
  Q_OBJECT
public:
  /** Construct a theme object */
  Theme(const char* name=0);
  virtual ~Theme();

  /** Get/set theme name. */
  const QString name(void) const { return mName; }
  virtual void setName(const QString newName);

  /** Returns path + name where theme is stored. */
  const QString fileName(void) { return mFileName; }

  /** Get preview pixmap. */
  const QPixmap& preview(void) const { return mPreview; }

  /** Write theme file. If no path is given the theme is stored in the default
   kde location with the given theme name. */
  virtual void save(const QString path=0);

  /** Read theme config file. If no path is given the theme is read from the default
   kde location with the given theme name. */
  virtual void load(const QString path=0);

  /** Apply current theme to the users Kde configuration. This updates several
   config files and executes the initialization programs. */
  virtual void install(void);

  /** Read current Kde configuration from several config files. */
  virtual void readCurrent(void);

  /** Description */
  const QString description(void) const { return mDescription; }
  virtual void setDescription(const QString);

  /** Color scheme colors. */
  QColor foregroundColor;
  QColor backgroundColor;
  QColor selectForegroundColor;
  QColor selectBackgroundColor;
  QColor activeForegroundColor;
  QColor activeBackgroundColor;
  QColor activeBlendColor;
  QColor inactiveForegroundColor;
  QColor inactiveBackgroundColor;
  QColor inactiveBlendColor;
  QColor windowForegroundColor;
  QColor windowBackgroundColor;
  int contrast;

  /** Theme packet installation options */
  bool instPanel, instIcons, instColors, instWindowBorder;
  bool instWindowTitlebar, instWallpapers, instSounds;

  /** Clear config contents */
  virtual void clear(void);

  /** Working directory. */
  static const QString workDir(void);

  /** Local themes directory. */
  static const QString themesDir(void);

  /** Global themes directory. */
  static const QString globalThemesDir(void);

  /** Create directory hierarchy, relative to given base directory
      or ~/.kde if none given. Returns true on success. */
  static bool mkdirhier(const char* dirHier, const char* baseDir=NULL);

  /** Test if group with given name exists. If notEmpty is true the
      method also tests if the group is not empty. */
  virtual bool hasGroup(const QString& name, bool notEmpty=false);

signals:
  /** This signal is emitted after import() or load() */
  void changed();

  /** This signal is emitted to ask for apply of all changes. */
  void apply();

protected:
  /** Write color entry to config file. */
  void writeColorEntry(KConfigBase*, const char* key, const QColor& color);

  /** Read color entry to config file. */
  const QColor& readColorEntry(KConfigBase*, const char* key, 
			       const QColor* pDefault=0L) const;

  /** Read values from config file. */
  virtual void readConfig(void);

  /** Write values to config file. */
  virtual void writeConfig(void);

  /** Create KConfig object and load fitting data. */
  virtual KConfig* openConfig(const QString appName) const;

  /** Installs file by calling cp. The source name is prepended
      with the theme work directory, the dest name is prepended
      with KApp::localkdedir(). Returns true on success. */
  virtual bool installFile(const QString& name, const QString& dest);

  /** Removes given file. If dirName is given and name is
      no absolute path, then dirName+"/"+name is removed. Does
      nothing if name is empty or null. */
  virtual void removeFile(const QString& name, const QString dirName=0);

  /** Install theme group. Returns number of installed files. */
  virtual int installGroup(const char* groupName);

  /** Apply color scheme change to all open windows. Taken from
      kdisplay / colorscm.cpp */
  virtual void colorSchemeApply(void);

  /** Load mappings file. */
  virtual void loadMappings(void);

  /** Execute all commands in the command list mCmdList. called from
      install(). */
  virtual void doCmdList(void);

  /** Called from installGroup() with the value of the ControlInstallCmd
      value. */
  virtual void installCmd(KSimpleConfig* cfg, const QString& cmd,
			  int installedFiles);

protected:
  QString mName, mDescription, mFileName;
  QPixmap mPreview;
  QString mConfigDir;
  KSimpleConfig* mMappings;
  QStrList mCmdList;
};

#endif /*THEME_H*/