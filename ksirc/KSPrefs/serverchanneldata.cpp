/**********************************************************************

	--- Qt Architect generated file ---

	File: serverchanneldata.cpp
	Last generated: Sat Jan 17 19:44:55 1998

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#include "serverchanneldata.h"

#define Inherited QFrame

#include <qbttngrp.h>
#include <qpushbt.h>

serverchanneldata::serverchanneldata
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name, 0 )
{
	QButtonGroup* dlgedit_ButtonGroup_2;
	dlgedit_ButtonGroup_2 = new("QButtonGroup") QButtonGroup( this, "ButtonGroup_2" );
	dlgedit_ButtonGroup_2->setGeometry( 10, 150, 380, 150 );
	dlgedit_ButtonGroup_2->setMinimumSize( 10, 10 );
	dlgedit_ButtonGroup_2->setMaximumSize( 32767, 32767 );
	dlgedit_ButtonGroup_2->setFrameStyle( 49 );
	dlgedit_ButtonGroup_2->setTitle( i18n("Channels") );
	dlgedit_ButtonGroup_2->setAlignment( 1 );

	QButtonGroup* dlgedit_ButtonGroup_1;
	dlgedit_ButtonGroup_1 = new("QButtonGroup") QButtonGroup( this, "ButtonGroup_1" );
	dlgedit_ButtonGroup_1->setGeometry( 10, 0, 380, 150 );
	dlgedit_ButtonGroup_1->setMinimumSize( 10, 10 );
	dlgedit_ButtonGroup_1->setMaximumSize( 32767, 32767 );
	dlgedit_ButtonGroup_1->setFrameStyle( 49 );
	dlgedit_ButtonGroup_1->setTitle( i18n("Servers") );
	dlgedit_ButtonGroup_1->setAlignment( 1 );

	LB_Servers = new("QListBox") QListBox( this, "ListBox_3" );
	LB_Servers->setGeometry( 20, 20, 200, 120 );
	LB_Servers->setMinimumSize( 10, 10 );
	LB_Servers->setMaximumSize( 32767, 32767 );
	LB_Servers->setFrameStyle( 51 );
	LB_Servers->setLineWidth( 2 );
	LB_Servers->setMultiSelection( FALSE );

	LB_Channels = new("QListBox") QListBox( this, "ListBox_4" );
	LB_Channels->setGeometry( 20, 170, 200, 120 );
	LB_Channels->setMinimumSize( 10, 10 );
	LB_Channels->setMaximumSize( 32767, 32767 );
	LB_Channels->setFrameStyle( 51 );
	LB_Channels->setLineWidth( 2 );
	LB_Channels->setMultiSelection( FALSE );

	QPushButton* dlgedit_PushButton_1;
	dlgedit_PushButton_1 = new("QPushButton") QPushButton( this, "PushButton_1" );
	dlgedit_PushButton_1->setGeometry( 230, 170, 150, 30 );
	dlgedit_PushButton_1->setMinimumSize( 10, 10 );
	dlgedit_PushButton_1->setMaximumSize( 32767, 32767 );
	connect( dlgedit_PushButton_1, SIGNAL(clicked()), SLOT(channels_delete()) );
	dlgedit_PushButton_1->setText( i18n("Delete") );
	dlgedit_PushButton_1->setAutoRepeat( FALSE );
	dlgedit_PushButton_1->setAutoResize( FALSE );

	B_AddChannels = new("QPushButton") QPushButton( this, "PushButton_2" );
	B_AddChannels->setGeometry( 230, 215, 150, 30 );
	B_AddChannels->setMinimumSize( 10, 10 );
	B_AddChannels->setMaximumSize( 32767, 32767 );
	connect( B_AddChannels, SIGNAL(clicked()), SLOT(channels_add()) );
	B_AddChannels->setText( i18n("Add") );
	B_AddChannels->setAutoRepeat( FALSE );
	B_AddChannels->setAutoResize( FALSE );

	QPushButton* dlgedit_PushButton_4;
	dlgedit_PushButton_4 = new("QPushButton") QPushButton( this, "PushButton_4" );
	dlgedit_PushButton_4->setGeometry( 230, 20, 150, 30 );
	dlgedit_PushButton_4->setMinimumSize( 10, 10 );
	dlgedit_PushButton_4->setMaximumSize( 32767, 32767 );
	connect( dlgedit_PushButton_4, SIGNAL(clicked()), SLOT(servers_delete()) );
	dlgedit_PushButton_4->setText( i18n("Delete") );
	dlgedit_PushButton_4->setAutoRepeat( FALSE );
	dlgedit_PushButton_4->setAutoResize( FALSE );

	B_AddServers = new("QPushButton") QPushButton( this, "PushButton_5" );
	B_AddServers->setGeometry( 230, 65, 150, 30 );
	B_AddServers->setMinimumSize( 10, 10 );
	B_AddServers->setMaximumSize( 32767, 32767 );
	connect( B_AddServers, SIGNAL(clicked()), SLOT(servers_add()) );
	B_AddServers->setText( i18n("Add") );
	B_AddServers->setAutoRepeat( FALSE );
	B_AddServers->setAutoResize( FALSE );

	SLE_Servers = new("QLineEdit") QLineEdit( this, "LineEdit_4" );
	SLE_Servers->setGeometry( 230, 110, 150, 30 );
	SLE_Servers->setMinimumSize( 10, 10 );
	SLE_Servers->setMaximumSize( 32767, 32767 );
	connect( SLE_Servers, SIGNAL(textChanged(const char*)), SLOT(servers_sle_update(const char*)) );
	SLE_Servers->setText( "" );
	SLE_Servers->setMaxLength( 32767 );
	SLE_Servers->setEchoMode( QLineEdit::Normal );
	SLE_Servers->setFrame( TRUE );

	SLE_Channels = new("QLineEdit") QLineEdit( this, "LineEdit_5" );
	SLE_Channels->setGeometry( 230, 260, 150, 30 );
	SLE_Channels->setMinimumSize( 10, 10 );
	SLE_Channels->setMaximumSize( 32767, 32767 );
	connect( SLE_Channels, SIGNAL(textChanged(const char*)), SLOT(channels_sle_update(const char*)) );
	SLE_Channels->setText( "" );
	SLE_Channels->setMaxLength( 32767 );
	SLE_Channels->setEchoMode( QLineEdit::Normal );
	SLE_Channels->setFrame( TRUE );

	dlgedit_ButtonGroup_2->insert( dlgedit_PushButton_1 );
	dlgedit_ButtonGroup_2->insert( B_AddChannels );

	dlgedit_ButtonGroup_1->insert( dlgedit_PushButton_4 );
	dlgedit_ButtonGroup_1->insert( B_AddServers );

	resize( 400,300 );
	setMinimumSize( 400, 300 );
	setMaximumSize( 400, 300 );
}


serverchanneldata::~serverchanneldata()
{
}
void serverchanneldata::channels_delete()
{
}
void serverchanneldata::channels_add()
{
}
void serverchanneldata::servers_delete()
{
}
void serverchanneldata::servers_add()
{
}
void serverchanneldata::servers_sle_update(const char*)
{
}
void serverchanneldata::channels_sle_update(const char*)
{
}
