/**********************************************************************

	--- Dlgedit generated file ---

	File: adddata.cpp
	Last generated: Fri Jun 20 07:42:44 1997

	DO NOT EDIT!!!  This file will be automatically
	regenerated by dlgedit.  All changes will be lost.

 *********************************************************************/

#include "adddata.h"

#define Inherited QDialog

#include <qframe.h>
#include <qlabel.h>
#include <kapp.h>
#include <klocale.h>

KAddDlgData::KAddDlgData
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	QFrame* tmpQFrame;
	tmpQFrame = new QFrame( this, "Frame_1" );
	tmpQFrame->setGeometry( 10, 10, 280, 90 );
	tmpQFrame->setFrameStyle( 34 );

	QLabel* tmpQLabel;
	tmpQLabel = new QLabel( this, "Label_1" );
	tmpQLabel->setGeometry( 20, 20, 100, 30 );
       tmpQLabel->setText( klocale->translate( "Task Name" ) );
	tmpQLabel->setAlignment( 265 );
	tmpQLabel->setMargin( -1 );

	tmpQLabel = new QLabel( this, "Label_2" );
	tmpQLabel->setGeometry( 20, 60, 100, 30 );
       tmpQLabel->setText( klocale->translate( "Accumutated time (in minutes)" ) );
	tmpQLabel->setAlignment( 1289 );
	tmpQLabel->setMargin( -1 );

	_taskName = new QLineEdit( this, "LineEdit_1" );
	_taskName->setGeometry( 130, 20, 150, 30 );
	_taskName->setText( "" );
	_taskName->setMaxLength( 32767 );
	_taskName->setEchoMode( QLineEdit::Normal );
	_taskName->setFrame( TRUE );

	_taskTime = new QLineEdit( this, "LineEdit_2" );
	_taskTime->setGeometry( 130, 60, 150, 30 );
	_taskTime->setText( "" );
	_taskTime->setMaxLength( 32767 );
	_taskTime->setEchoMode( QLineEdit::Normal );
	_taskTime->setFrame( TRUE );

	_okButton = new QPushButton( this, "PushButton_1" );
	_okButton->setGeometry( 40, 110, 100, 30 );
	connect( _okButton, SIGNAL(clicked()), SLOT(okClicked()) );
	_okButton->setText( "&OK" );
	_okButton->setAutoRepeat( FALSE );
	_okButton->setAutoResize( FALSE );

	_cancelButton = new QPushButton( this, "PushButton_2" );
	_cancelButton->setGeometry( 160, 110, 100, 30 );
	connect( _cancelButton, SIGNAL(clicked()), SLOT(cancelClicked()) );
       _cancelButton->setText( klocale->translate( "&Cancel" ) );
	_cancelButton->setAutoRepeat( FALSE );
	_cancelButton->setAutoResize( FALSE );

	resize( 300, 150 );
}


KAddDlgData::~KAddDlgData()
{
}
void KAddDlgData::okClicked()
{
}
void KAddDlgData::cancelClicked()
{
}
