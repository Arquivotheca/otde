/* -*- C++ -*-
 *            kPPP: A pppd front end for the KDE project
 *
 * $Id$
 * 
 *            Copyright (C) 1997 Bernd Johannes Wuebben 
 *                   wuebben@math.cornell.edu
 *
 * based on EzPPP:
 * Copyright (C) 1997  Jay Painter
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _ACCOUNTS_H_
#define _ACCOUNTS_H_

#include "acctselect.h"
#include <qwidget.h>
#include <qpushbt.h>
#include <qlistbox.h>
#include "pppdata.h"
#include "edit.h"

class AccountWidget : public QWidget {
Q_OBJECT
public:
  AccountWidget( QWidget *parent=0, const char *name=0 );
  ~AccountWidget() {}

  //private slots:
 public slots:
  void editaccount();
  void copyaccount();
  void newaccount();
  void deleteaccount();
  void slotListBoxSelect(int);
  void resetClicked();
  void viewLogClicked();

private:
  int doTab();

private:
  
  QTabDialog *tabWindow;
  DialWidget *dial_w;
  AccountingSelector *acct;
  IPWidget *ip_w;
  DNSWidget *dns_w;
  GatewayWidget *gateway_w;
  ScriptWidget *script_w;

  QPushButton *reset;
  QPushButton *log;
  QLabel *costlabel;
  QLineEdit *costedit;
  
  QGroupBox *box;
  QListBox *accountlist_l;
  QPushButton *edit_b;
  QPushButton *copy_b;
  QPushButton *new_b;
  QPushButton *delete_b;
};

#endif






