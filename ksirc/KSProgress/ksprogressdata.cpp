/**********************************************************************

	--- Qt Architect generated file ---

	File: ksprogressdata.cpp
	Last generated: Thu Dec 18 09:09:53 1997

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#include "ksprogressdata.h"

#define Inherited QFrame

#include <qpushbt.h>

ksprogressData::ksprogressData
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name, 0 )
{
	progress = new KProgress( this, "User_1" );
	progress->setGeometry( 10, 60, 230, 20 );
	progress->setMinimumSize( 10, 10 );
	progress->setMaximumSize( 32767, 32767 );

	QPushButton* dlgedit_PushButton_1;
	dlgedit_PushButton_1 = new QPushButton( this, "PushButton_1" );
	dlgedit_PushButton_1->setGeometry( 70, 90, 100, 30 );
	dlgedit_PushButton_1->setMinimumSize( 10, 10 );
	dlgedit_PushButton_1->setMaximumSize( 32767, 32767 );
	connect( dlgedit_PushButton_1, SIGNAL(clicked()), SLOT(cancelPressed()) );
	dlgedit_PushButton_1->setText( "&Cancel" );
	dlgedit_PushButton_1->setAutoRepeat( FALSE );
	dlgedit_PushButton_1->setAutoResize( FALSE );

	fileName = new QLabel( this, "Label_1" );
	fileName->setGeometry( 10, 10, 230, 20 );
	fileName->setMinimumSize( 10, 10 );
	fileName->setMaximumSize( 32767, 32767 );
	fileName->setText( "fileName" );
	fileName->setAlignment( 289 );
	fileName->setMargin( -1 );

	transferStatus = new QLabel( this, "Label_2" );
	transferStatus->setGeometry( 10, 35, 230, 20 );
	transferStatus->setMinimumSize( 10, 10 );
	transferStatus->setMaximumSize( 32767, 32767 );
	transferStatus->setText( "XferStatus" );
	transferStatus->setAlignment( 289 );
	transferStatus->setMargin( -1 );

	resize( 250,130 );
	setMinimumSize( 250, 140 );
	setMaximumSize( 250, 140 );
}


ksprogressData::~ksprogressData()
{
}
void ksprogressData::cancelPressed()
{
}