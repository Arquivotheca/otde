/* -*- C++ -*-
 *
 *   CommandDlg.h - Command widget
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
 *   MERCHANTABLITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
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
 * Revision 1.2  1997/11/23 22:28:06  leconte
 * - $Id$ and $Log$ added in the headers
 * - Patch from C.Czezatke applied (preparation of KProcess new version)
 *
 */

#ifndef __CommandDlg_h__
#define __CommandDlg_h__

#include <kapp.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qlined.h>
#include <qpushbt.h>
#include <qlayout.h>
#include <qchkbox.h>
#include <qgrpbox.h>
#include <qcombo.h>
#include <qmlined.h>
#include <kprocess.h>

class CommandCfgDlg;


/**
 * Implements the inside of the tabCtrl with mainly
 * three items :
 *  - a LineEdit to type in a hostname
 *  - a Go/Stop button
 *  - a MultiLineEdit to receive the output
 */
class CommandDlg: public QWidget
{
  Q_OBJECT

public:
  CommandDlg(QString commandName, 
	     QWidget* parent = NULL, const char* name = NULL);
  virtual  ~CommandDlg();

  // callback to inform us that we are shown
  void      tabSelected();
  
  // callback to inform us that we have to kill the command
  void      tabDeselected();

protected:
  virtual bool     checkInput(QString *);
  virtual void     buildCommandLine(QString);
  
  // Widgets
  QLabel*          commandLbl1;
  QLabel*          commandLblNoBin;
  QLineEdit*       commandArgs;
  //QComboBox*       commandArgs; NOT YET...
  QPushButton*     commandGoBtn;
  QPushButton*     commandStopBtn;
  QMultiLineEdit*  commandTextArea;
  
  // Layout
  QBoxLayout*      layoutNoBin;	// Vertical
  QBoxLayout*      layout1;	// Vertical
  QBoxLayout*      layout2;	// Horizontal
  
  bool             isGoBtnEnabled;
  // our child
  KProcess         childProcess;

  // Have we found the command binary ?
  bool commandFound;

  // First time we are lauched ?
  bool firstTimeLauching;

  // The group name in the configfile (computed from tab name)
  QString         configGroupName;


public slots:
  void slotCmdStdout(KProcess *, char *, int);
  void slotProcessDead(KProcess *);

  /**
   * clear output window
   */
  void clearOutput();

  /**
   * select all in output window
   */
  void selectAll() {commandTextArea->selectAll();};

protected slots:
    void slotEnableGoBtn(const char *);
    void slotStopCommand();
    void slotLauchCommand();

};

class CommandCfgDlg: public QWidget
{
  Q_OBJECT

public:
  CommandCfgDlg(const char *tcs, QWidget* parent = NULL, 
		const char* name = NULL);
  ~CommandCfgDlg();

  friend class CommandDlg;

  virtual QWidget *makeWidget(QWidget* parent, bool makeLayouts = TRUE);
  virtual void deleteWidget();

  QString tabName() {return tabCaptionString;};

  /**
   * commit changes to the configfile
   * 
   * @return if the change have been done
   * @see cancelChanges
   */
  virtual bool commitChanges();
  
  /**
   * cancel changes to the configfile
   *
   * @see commitChanges
   */
  virtual void cancelChanges();

  /**
   * This is called just before the OptionDlg is shown
   */
  void readConfig();

protected:

  // The configWidget
  QWidget         *cfgWidget;
  QFrame          *cfgWarning;
  QLabel          *cfgWarningLbl;
  QLabel          *cfgWarningPm;
  QBoxLayout      *cfgWarningLayout;
  QGroupBox       *cfgBinGB;
  QLabel          *cfgBinNameLbl, *cfgBinArgLbl;
  QLineEdit       *cfgBinNameLE, *cfgBinArgLE;
  QBoxLayout      *cfgLayoutTB;
  QGridLayout     *cfgLayoutGB;
  QPushButton     *cfgHelpBtn;

  QString         tabCaptionString;
  QString         configGroupName;

};

#endif // __CommandDlg_h__


