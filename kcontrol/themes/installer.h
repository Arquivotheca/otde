/*
 * installer.h
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
#ifndef INSTALLER_H
#define INSTALLER_H

#include <kcontrol.h>

class QGridLayout;
class QListBox;
class QPushButton;
class QLabel;
class Theme;
class QMultiLineEdit;

#define InstallerInherited KConfigWidget
class Installer : public KConfigWidget
{
  Q_OBJECT
public:
  Installer(QWidget *parent=0, const char *aName=0, bool aInit=FALSE);
  ~Installer();

  virtual void loadSettings();
  virtual void applySettings();

protected slots:
  virtual void slotImport();
  virtual void slotExport();
  virtual void slotNew();
  virtual void slotRemove();
  virtual void slotThemeChanged();
  virtual void slotSetTheme(int);

protected:
  /** Scan Themes directory for available theme packages */
  virtual void readThemesList(void);

private:
  bool mGui;
  QGridLayout *mGrid;
  QListBox *mThemesList;
  QPushButton *mBtnNew, *mBtnExport, *mBtnImport, *mBtnRemove;
  QMultiLineEdit *mText;
  QLabel *mPreview;
};

#endif /*INSTALLER_H*/

