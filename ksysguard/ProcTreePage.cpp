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

#include <signal.h>
#include <errno.h>

#include <qstring.h>
#include <qmessagebox.h>

#include "ProcTreePage.moc"

#define NONE -1

typedef struct
{
	const char* label;
	OSProcessList::SORTKEY sortMethod;
} SortButton;

/*
 * This array defines the items of the sort selection combo box. The labels
 * are only provided to enhance readability. They are not used for the 
 * creation of the combo box since the i18n() mechanism needs to work on
 * fixed strings.
 */
static SortButton SortButtons[] =
{
	{ "Sort by ID", OSProcessList::SORTBY_PID },
	{ "Sort by Name", OSProcessList::SORTBY_NAME },
	{ "Sort by Owner (UID)", OSProcessList::SORTBY_UID }
};

static const int NoSortButtons = sizeof(SortButtons) / sizeof(SortButton);

ProcTreePage::ProcTreePage(QWidget* parent = 0, const char* name = 0)
	: QWidget(parent, name)
{
	// surrounding box with title "Running Processes"
	box = new QGroupBox(this, "pTree_box"); 
	CHECK_PTR(box); 
	box->setTitle(i18n("Running Processes"));

	// the tree handling widget
	pTree = new ProcessTree(this, "pTree"); 
	CHECK_PTR(pTree); 
	pTree->setExpandLevel(20); 
	pTree->setSmoothScrolling(TRUE);
	connect(pTree, SIGNAL(popupMenu(QPoint)),
			parent, SLOT(popupMenu(QPoint)));

	/*
	 * four buttons which should appear on the sheet (just below the tree box)
	 */

	// "Refresh Now" button
	bRefresh = new QPushButton(i18n("Refresh Now"), this,
									 "pTree_bRefresh");
	CHECK_PTR(bRefresh);
	connect(bRefresh, SIGNAL(clicked()), this, SLOT(update()));

	// "Change Root" button
	bRoot = new QPushButton(i18n("Change Root"), this, "pTree_bRoot");
	CHECK_PTR(bRoot);
	connect(bRoot, SIGNAL(clicked()), this, SLOT(changeRoot()));

	// "Kill Task" button
	bKill = new QPushButton(i18n("Kill Task"), this, "pTree_bKill");
	CHECK_PTR(bKill);
	connect(bKill, SIGNAL(clicked()), this, SLOT(killTask()));

	// Sorting Method combo button
	cbSort = new QComboBox(this, "pTree_cbSort");
	CHECK_PTR(cbSort);
	// These combo box labels and the SortButton array must be kept in sync!
	cbSort->insertItem(i18n("Sort by ID"));
	cbSort->insertItem(i18n("Sort by Name"));
	cbSort->insertItem(i18n("Sort by Owner (UID)"));
	connect(cbSort, SIGNAL(activated(int)),
			SLOT(handleSortChange(int)));

	strcpy(cfgkey_pTreeSort, "pTreeSort");

	// restore sort method for pTree...
	sortby = 0;
	loadSetting(sortby, "pTreeSort");
	pTree->setSortMethod(SortButtons[sortby].sortMethod);
	cbSort->setCurrentItem(sortby);
    
	// create process tree
	pTree->update();
}

void
ProcTreePage::resizeEvent(QResizeEvent* ev)
{
    QWidget::resizeEvent(ev);

    int w = width();
    int h = height();
   
	box->setGeometry(5, 5, w - 10, h - 20);

   	pTree->setGeometry(10, 30, w - 20, h - 90);

	cbSort->setGeometry(10, h - 50, 160, 25);
	bRefresh->setGeometry(w - 300, h - 50, 90, 25);
	bRoot->setGeometry(w - 200, h - 50, 90, 25);
	bKill->setGeometry(w - 100, h - 50, 90, 25);
}

void
ProcTreePage::handleSortChange(int idx)
{
	pTree->setSortMethod(SortButtons[sortby = idx].sortMethod);
	pTree->update();
}

void 
ProcTreePage::killTask()
{
	int pid = selectionPid();
	if (pid < 0)
		return;

	emit(killProcess(pid));

	pTree->update();
}
