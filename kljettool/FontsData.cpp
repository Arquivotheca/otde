/**********************************************************************

	--- Dlgedit generated file ---

	File: FontsData.cpp
	Last generated: Tue Jun 24 22:20:39 1997

	DO NOT EDIT!!!  This file will be automatically
	regenerated by dlgedit.  All changes will be lost.

 *********************************************************************/

#include "FontsData.h"

#define Inherited QDialog

#include <qframe.h>
#include <qlabel.h>
#include <kapp.h>
#include <klocale.h>

FontsData::FontsData
(
	QWidget* parent,
	const char* name
)
	:
	Inherited( parent, name )
{
	QFrame* tmpQFrame;
	tmpQFrame = new QFrame( this, "Frame_4" );
	tmpQFrame->setGeometry( 10, 10, 320, 235 );
	tmpQFrame->setFrameStyle( 49 );

	QLabel* tmpQLabel;
	tmpQLabel = new QLabel( this, "LanguageLabel" );
	tmpQLabel->setGeometry( 20, 30, 90, 20 );
	tmpQLabel->setText( i18n("Language") );
	tmpQLabel->setAlignment( 290 );
	tmpQLabel->setMargin( -1 );

	LanguageComboBox = new QComboBox( FALSE, this, "ComboBox_2" );
	LanguageComboBox->setGeometry( 130, 30, 100, 25 );

	connect( LanguageComboBox, SIGNAL(activated(const char*)), 
		 SLOT(LanguageComboBoxSelected(const char*)) );

	LanguageComboBox->setSizeLimit( 10 );
	LanguageComboBox->setAutoResize( FALSE );
	LanguageComboBox->insertItem( "pcl" );
	LanguageComboBox->insertItem( "postscript" );
	LanguageComboBox->insertItem( "escp" );
	LanguageComboBox->insertItem( "auto" );

	tmpQLabel = new QLabel( this, "SymbolSetLabel" );
	tmpQLabel->setGeometry( 20, 70, 90, 20 );
	tmpQLabel->setText( i18n("Symbol Set") );
	tmpQLabel->setAlignment( 290 );
	tmpQLabel->setMargin( -1 );

	SymbolSetCombo = new QComboBox( FALSE, this, "SymbolCombo" );
	SymbolSetCombo->setGeometry( 130, 70, 100, 25 );

	connect( SymbolSetCombo, SIGNAL(activated(const char*)), 
		 SLOT(SymbolSetComboSelected(const char*)) );

	SymbolSetCombo->setSizeLimit( 8 );
	SymbolSetCombo->setAutoResize( FALSE );
	//	SymbolSetCombo->insertItem( "PCB" );
	SymbolSetCombo->insertItem( "Desktop" );
	SymbolSetCombo->insertItem( "ISO4" );
	SymbolSetCombo->insertItem( "ISO6" );
	SymbolSetCombo->insertItem( "ISO11" );
	SymbolSetCombo->insertItem( "ISO15" );
	SymbolSetCombo->insertItem( "ISO17" );
	SymbolSetCombo->insertItem( "ISO21" );
	SymbolSetCombo->insertItem( "ISO60" );
	SymbolSetCombo->insertItem( "ISO69" );
	SymbolSetCombo->insertItem( "ISOL1" );
	SymbolSetCombo->insertItem( "ISOL2" );
	SymbolSetCombo->insertItem( "ISOL5" );
	SymbolSetCombo->insertItem( "Legal" );
	SymbolSetCombo->insertItem( "Math8" );
	SymbolSetCombo->insertItem( "MSPubl" );
	SymbolSetCombo->insertItem( "PC8" );
	SymbolSetCombo->insertItem( "PC8DN" );
	SymbolSetCombo->insertItem( "PC850" );
	SymbolSetCombo->insertItem( "PC852" );
	SymbolSetCombo->insertItem( "PC8TK" );
	SymbolSetCombo->insertItem( "Pifont" );
	SymbolSetCombo->insertItem( "PSMath" );
	SymbolSetCombo->insertItem( "PSText" );
	SymbolSetCombo->insertItem( "Roman8" );
	SymbolSetCombo->insertItem( "VNIntl" );
	SymbolSetCombo->insertItem( "VNMath" );
	SymbolSetCombo->insertItem( "VNUS" );
	SymbolSetCombo->insertItem( "Win30" );
	SymbolSetCombo->insertItem( "WinL1" );
	SymbolSetCombo->insertItem( "WinL2" );
	SymbolSetCombo->insertItem( "WinL5" );

	tmpQLabel = new QLabel( this, "Label_6" );
	tmpQLabel->setGeometry( 20, 110, 90, 25 );
	tmpQLabel->setText(i18n( "Font") );
	tmpQLabel->setAlignment( 290 );
	tmpQLabel->setMargin( -1 );

	Font = new QLineEdit( this, "LineEdit_3" );
	Font->setGeometry( 130, 110, 100, 25 );
	Font->setText( "0" );
	Font->setMaxLength( 32767 );
	Font->setEchoMode( QLineEdit::Normal );
	Font->setFrame( TRUE );
	connect(Font,SIGNAL(textChanged(const char*)),this,
			    SLOT(Fontnumberselected(const char*)));

	tmpQLabel = new QLabel( this, "Label_7" );
	tmpQLabel->setGeometry( 240, 150, 40, 30 );
	tmpQLabel->setText( "cpi" );
	tmpQLabel->setAlignment( 289 );
	tmpQLabel->setMargin( -1 );

	tmpQLabel = new QLabel( this, "Label_8" );
	tmpQLabel->setGeometry( 20, 150, 90, 30 );
	tmpQLabel->setText(i18n( "Pitch") );
	tmpQLabel->setAlignment( 290 );
	tmpQLabel->setMargin( -1 );

	Pitch = new QLineEdit( this, "LineEdit_4" );
	Pitch->setGeometry( 130, 150, 100, 25 );
	Pitch->setText( "10.00" );
	Pitch->setMaxLength( 32767 );
	Pitch->setEchoMode( QLineEdit::Normal );
	Pitch->setFrame( TRUE );
	connect(Pitch,SIGNAL(textChanged(const char*)),this,
			    SLOT(Pitchselected(const char*)));


	tmpQLabel = new QLabel( this, "Label_9" );
	tmpQLabel->setGeometry( 20, 185, 90, 30 );
	tmpQLabel->setText(i18n( "Point Size") );
	tmpQLabel->setAlignment( 290 );
	tmpQLabel->setMargin( -1 );

	PointSize = new QLineEdit( this, "LineEdit_5" );
	PointSize->setGeometry( 130, 190, 100, 25 );
	PointSize->setText( "12.0" );
	PointSize->setMaxLength( 32767 );
	PointSize->setEchoMode( QLineEdit::Normal );
	PointSize->setFrame( TRUE );
	connect(PointSize,SIGNAL(textChanged(const char*)),this,
			    SLOT(PointSizeselected(const char*)));

	tmpQLabel = new QLabel( this, "Label_10" );
	tmpQLabel->setGeometry( 240, 185, 50, 30 );
	tmpQLabel->setText(i18n( "points") );
	tmpQLabel->setAlignment( 289 );
	tmpQLabel->setMargin( -1 );

	resize( 330, 255 );
}


FontsData::~FontsData()
{
}


void FontsData::LanguageComboBoxSelected(const char* language)
{

  if(!data)
    return;

  data->Language = language;

}


void FontsData::SymbolSetComboSelected(const char* symset)
{

  if(!data)
    return;

  data->Symset = symset;

}


void FontsData::Fontnumberselected(const char* fn){


  if(!data)
    return;

  data->Fontnumber = fn;
  

}

void FontsData::Pitchselected(const char* pt){

  if(!data)
    return;

  data->Pitch = pt;	

}


void FontsData::PointSizeselected(const char* ps){


  if(!data)
    return;

  data->Ptsize =  ps;
}