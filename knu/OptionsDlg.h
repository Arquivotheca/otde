/* -*- C++ -*-
 *   OptionsDlg.h - Options window (widget)
 * 
 *   part of knu: KDE network utilities
 *
 *   Copyright (C) 1997  Bertrand Leconte
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
/*
 * $Id$
 *
 * $Log$
 * Revision 1.3  1997/12/07 23:44:20  leconte
 * - $Id$ and $Log$ added in the headers
 *   by Conrad Sanderson)
 * - added browse button to the option dialog (for binary selection)
 * - code clean-up
 * - better fallback to"nslookup" if "host" is not found
 *
 * Revision 1.2  1997/11/23 22:28:11  leconte
 * - Id and Log added in the headers
 * - Patch from C.Czezatke applied (preparation of KProcess new version)
 *
 */


#ifndef __OptionsDlg_h__
#define __OptionsDlg_h__

#include <qlayout.h>
#include <qlined.h>
#include <qlabel.h>
#include <qframe.h>
#include <qpushbt.h>
#include <qdialog.h>
#include <qgrpbox.h>
#include <qbttngrp.h>
//class OptionsDlg: public QFrame
//class OptionsDlg: public QSemiModal
#include <qradiobt.h>
#include <qchkbox.h>
#include <qsemimodal.h> 
#include <ktabctl.h>

#include "CommandDlg.h"

class OptionsDlg: public QDialog
{
  Q_OBJECT;

public:
  OptionsDlg(CommandCfgDlg **, int, 
	     QWidget* parent = NULL, const char* name = NULL);
  virtual ~OptionsDlg();
  //void show();

public slots:
  void slotOkBtn();
  void slotHelpBtn();
  //CommandDlg **getPages(int *);
  
  void slotCancelBtn();
  void closeEvent(QCloseEvent*);


  // widgets
  void optionsClosed();

private:
  CommandCfgDlg  **configPages;
  int              pagesNumber;

   // widgets
  QPushButton *bBtnOk, *bBtnHelp, *bBtnCancel;
  
  // layout
  QBoxLayout  *layout1;
  QBoxLayout  *layoutB;

  // KTabCtl
  KTabCtl     *cfgTabCtrl;
};

#endif  // __OptionsDlg_h__
