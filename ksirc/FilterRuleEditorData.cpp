/**********************************************************************

	--- Qt Architect generated file ---

	File: FilterRuleEditorData.cpp
	Last generated: Fri Jul 24 19:34:15 1998

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#include "FilterRuleEditorData.h"

#define Inherited QDialog

#include <qlabel.h>
#include <qpushbt.h>

FilterRuleEditorData::FilterRuleEditorData
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name, 118784 )
{
	QLabel* dlgedit_Label_1;
	dlgedit_Label_1 = new("QLabel") QLabel( this, "Label_1" );
	dlgedit_Label_1->setGeometry( 140, 10, 80, 30 );
	dlgedit_Label_1->setMinimumSize( 0, 0 );
	dlgedit_Label_1->setMaximumSize( 32767, 32767 );
	dlgedit_Label_1->setText( i18n("Description:") );
	dlgedit_Label_1->setAlignment( 289 );
	dlgedit_Label_1->setMargin( -1 );

	LineTitle = new("QLineEdit") QLineEdit( this, "LineEdit_1" );
	LineTitle->setGeometry( 220, 10, 280, 30 );
	LineTitle->setMinimumSize( 0, 0 );
	LineTitle->setMaximumSize( 32767, 32767 );
	LineTitle->setText( "" );
	LineTitle->setMaxLength( 80 );
	LineTitle->setEchoMode( QLineEdit::Normal );
	LineTitle->setFrame( TRUE );

	QLabel* dlgedit_Label_4;
	dlgedit_Label_4 = new("QLabel") QLabel( this, "Label_4" );
	dlgedit_Label_4->setGeometry( 140, 50, 70, 30 );
	dlgedit_Label_4->setMinimumSize( 0, 0 );
	dlgedit_Label_4->setMaximumSize( 32767, 32767 );
	dlgedit_Label_4->setText( i18n("Match:") );
	dlgedit_Label_4->setAlignment( 289 );
	dlgedit_Label_4->setMargin( -1 );

	LineSearch = new("QLineEdit") QLineEdit( this, "LineEdit_2" );
	LineSearch->setGeometry( 220, 50, 280, 30 );
	LineSearch->setMinimumSize( 0, 0 );
	LineSearch->setMaximumSize( 32767, 32767 );
	LineSearch->setText( "" );
	LineSearch->setMaxLength( 80 );
	LineSearch->setEchoMode( QLineEdit::Normal );
	LineSearch->setFrame( TRUE );

	QLabel* dlgedit_Label_7;
	dlgedit_Label_7 = new("QLabel") QLabel( this, "Label_7" );
	dlgedit_Label_7->setGeometry( 140, 90, 80, 30 );
	dlgedit_Label_7->setMinimumSize( 0, 0 );
	dlgedit_Label_7->setMaximumSize( 32767, 32767 );
	dlgedit_Label_7->setText( i18n("From:") );
	dlgedit_Label_7->setAlignment( 289 );
	dlgedit_Label_7->setMargin( -1 );

	QLabel* dlgedit_Label_9;
	dlgedit_Label_9 = new("QLabel") QLabel( this, "Label_9" );
	dlgedit_Label_9->setGeometry( 140, 130, 70, 30 );
	dlgedit_Label_9->setMinimumSize( 0, 0 );
	dlgedit_Label_9->setMaximumSize( 32767, 32767 );
	dlgedit_Label_9->setText( i18n("To:") );
	dlgedit_Label_9->setAlignment( 289 );
	dlgedit_Label_9->setMargin( -1 );

	LineFrom = new("QLineEdit") QLineEdit( this, "LineEdit_3" );
	LineFrom->setGeometry( 220, 90, 280, 30 );
	LineFrom->setMinimumSize( 0, 0 );
	LineFrom->setMaximumSize( 32767, 32767 );
	LineFrom->setText( "" );
	LineFrom->setMaxLength( 32767 );
	LineFrom->setEchoMode( QLineEdit::Normal );
	LineFrom->setFrame( TRUE );

	LineTo = new("QLineEdit") QLineEdit( this, "LineEdit_4" );
	LineTo->setGeometry( 220, 130, 280, 30 );
	LineTo->setMinimumSize( 0, 0 );
	LineTo->setMaximumSize( 32767, 32767 );
	LineTo->setText( "" );
	LineTo->setMaxLength( 80 );
	LineTo->setEchoMode( QLineEdit::Normal );
	LineTo->setFrame( TRUE );

	ApplyButton = new("QPushButton") QPushButton( this, "PushButton_1" );
	ApplyButton->setGeometry( 390, 170, 110, 30 );
	ApplyButton->setMinimumSize( 10, 10 );
	ApplyButton->setMaximumSize( 32767, 32767 );
	connect( ApplyButton, SIGNAL(clicked()), SLOT(OkPressed()) );
	ApplyButton->setText( i18n("&Modify") );
	ApplyButton->setAutoRepeat( FALSE );
	ApplyButton->setAutoResize( FALSE );
	ApplyButton->setAutoDefault( TRUE );

	QPushButton* dlgedit_PushButton_2;
	dlgedit_PushButton_2 = new("QPushButton") QPushButton( this, "PushButton_2" );
	dlgedit_PushButton_2->setGeometry( 390, 210, 110, 30 );
	dlgedit_PushButton_2->setMinimumSize( 10, 10 );
	dlgedit_PushButton_2->setMaximumSize( 32767, 32767 );
	connect( dlgedit_PushButton_2, SIGNAL(clicked()), SLOT(closePressed()) );
	dlgedit_PushButton_2->setText( i18n("&Close") );
	dlgedit_PushButton_2->setAutoRepeat( FALSE );
	dlgedit_PushButton_2->setAutoResize( FALSE );

	QPushButton* dlgedit_PushButton_7;
	dlgedit_PushButton_7 = new("QPushButton") QPushButton( this, "PushButton_7" );
	dlgedit_PushButton_7->setGeometry( 270, 170, 110, 30 );
	dlgedit_PushButton_7->setMinimumSize( 10, 10 );
	dlgedit_PushButton_7->setMaximumSize( 32767, 32767 );
	connect( dlgedit_PushButton_7, SIGNAL(clicked()), SLOT(newRule()) );
	dlgedit_PushButton_7->setText( i18n("New Rule") );
	dlgedit_PushButton_7->setAutoRepeat( FALSE );
	dlgedit_PushButton_7->setAutoResize( FALSE );

	deleteButton = new("QPushButton") QPushButton( this, "PushButton_8" );
	deleteButton->setGeometry( 150, 170, 110, 30 );
	deleteButton->setMinimumSize( 10, 10 );
	deleteButton->setMaximumSize( 32767, 32767 );
	connect( deleteButton, SIGNAL(clicked()), SLOT(deleteRule()) );
	deleteButton->setText( i18n("&Delete") );
	deleteButton->setAutoRepeat( FALSE );
	deleteButton->setAutoResize( FALSE );

	QPushButton* dlgedit_PushButton_9;
	dlgedit_PushButton_9 = new("QPushButton") QPushButton( this, "PushButton_9" );
	dlgedit_PushButton_9->setGeometry( 10, 210, 55, 30 );
	dlgedit_PushButton_9->setMinimumSize( 10, 10 );
	dlgedit_PushButton_9->setMaximumSize( 32767, 32767 );
	connect( dlgedit_PushButton_9, SIGNAL(clicked()), SLOT(raiseRule()) );
	dlgedit_PushButton_9->setText( i18n("Up") );
	dlgedit_PushButton_9->setAutoRepeat( FALSE );
	dlgedit_PushButton_9->setAutoResize( FALSE );

	QPushButton* dlgedit_PushButton_10;
	dlgedit_PushButton_10 = new("QPushButton") QPushButton( this, "PushButton_10" );
	dlgedit_PushButton_10->setGeometry( 75, 210, 55, 30 );
	dlgedit_PushButton_10->setMinimumSize( 10, 10 );
	dlgedit_PushButton_10->setMaximumSize( 32767, 32767 );
	connect( dlgedit_PushButton_10, SIGNAL(clicked()), SLOT(lowerRule()) );
	dlgedit_PushButton_10->setText( i18n("Down") );
	dlgedit_PushButton_10->setAutoRepeat( FALSE );
	dlgedit_PushButton_10->setAutoResize( FALSE );

	RuleList = new("QListBox") QListBox( this, "ListBox_2" );
	RuleList->setGeometry( 10, 10, 120, 190 );
	RuleList->setMinimumSize( 10, 10 );
	RuleList->setMaximumSize( 32767, 32767 );
	connect( RuleList, SIGNAL(highlighted(int)), SLOT(newHighlight(int)) );
	connect( RuleList, SIGNAL(selected(int)), SLOT(newHighlight(int)) );
	RuleList->setFrameStyle( 51 );
	RuleList->setLineWidth( 2 );
	RuleList->setDragSelect( FALSE );
	RuleList->setAutoScroll( FALSE );
	RuleList->setAutoBottomScrollBar( FALSE );
	RuleList->setSmoothScrolling( FALSE );
	RuleList->setMultiSelection( FALSE );

	resize( 510,250 );
	setMinimumSize( 0, 0 );
	setMaximumSize( 32767, 32767 );
}


FilterRuleEditorData::~FilterRuleEditorData()
{
}
void FilterRuleEditorData::OkPressed()
{
}
void FilterRuleEditorData::closePressed()
{
}
void FilterRuleEditorData::newRule()
{
}
void FilterRuleEditorData::deleteRule()
{
}
void FilterRuleEditorData::raiseRule()
{
}
void FilterRuleEditorData::lowerRule()
{
}
void FilterRuleEditorData::newHighlight(int)
{
}
