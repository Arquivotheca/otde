/*
    KTop, a taskmanager and cpu load monitor
   
    Copyright (C) 1997 Bernd Johannes Wuebben
                       wuebben@math.cornell.edu

    Copyright (C) 1998 Nicolas Leclercq
                       nicknet@planete.net
    
	Copyright (c) 1999 Chris Schlaeger
	                   cs@axys.de
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

// $Id$

#ifndef _TaskMan_h_
#define _TaskMan_h_

#include <signal.h>

#include <qwidget.h>
#include <qtabdialog.h>

#include <ktablistbox.h>

#include "settings.h"
#include "IconList.h"
#include "ProcessList.h"
#include "ProcessTree.h"
#include "ProcListPage.h"
#include "ProcTreePage.h"
#include "PerfMonPage.h"

/**
 * This class creates the main dialog widget of ktop. It consists of a 
 * 3 entry tab dialog. The tabs contain a process list, a process tree and
 * a performance meter.
 */
class TaskMan : public QTabDialog
{
	Q_OBJECT

public:
	enum 
	{
		MENU_ID_SIGINT = 500,
		MENU_ID_SIGQUIT,
		MENU_ID_SIGTERM,
		MENU_ID_SIGKILL,
		MENU_ID_SIGUSR1,
		MENU_ID_SIGUSR2,
	    MENU_ID_RENICE 
	};
	enum 
	{
		PAGE_PLIST = 0, 
		PAGE_PTREE, 
		PAGE_PERF
	};
	enum 
	{
		MENU_ID_PROP = 100,
		MENU_ID_KILL
	};

	TaskMan(QWidget* parent = 0, const char* name = 0, int sfolder = 0);
    ~TaskMan()
	{
		delete procListPage;
	}
     
	int setUpdateInterval(int);
	int getUpdateInterval();
	void invokeSettings();
	void raiseStartUpPage();
	void saveSettings();

	static void TaskMan_initIconList(); 
	static void TaskMan_clearIconList();
	static const QPixmap* TaskMan_getProcIcon(const char*);

public slots:
	void pSigHandler(int);
	void tabBarSelected(int);

	void popupMenu(int, int)
	{
		pSig->popup(QCursor::pos());
	}
	void popupMenu(QPoint)
	{
		pSig->popup(QCursor::pos());
	}
	void killProcess(int pid)
	{
		killProcess(pid, SIGKILL, "SIGKILL");
	}

private:
	void reniceProcess(int pid);
	void killProcess(int, int sig, const char*);

	ProcListPage* procListPage;

	ProcTreePage* procTreePage;

	PerfMonPage* perfMonPage;

    QWidget* pages[3];
	
	QPopupMenu* pSig;
	AppSettings* settings;

	int startup_page;
	bool restoreStartupPage;
};

#endif
