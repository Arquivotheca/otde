//
// KFM  Options
//
// (c) Martin R. Jones 1996
// (c) Bernd Wuebben 1998

#include <qbttngrp.h>
#include <qradiobt.h>
#include <qpushbt.h>
#include <qcombo.h>
#include <qlabel.h>
#include <qchkbox.h>
#include <qlayout.h>//CT - 12Nov1998
#include <kapp.h>
#include <kconfig.h>
#include <X11/Xlib.h>

#include "kcolorbtn.h"
#include "htmlopts.h"
#include "htmlopts.moc"
#include <klocale.h>
#include "config-kfm.h"

#include "root.h" // for the gridsize definitions

//-----------------------------------------------------------------------------

KFontOptions::KFontOptions( QWidget *parent, const char *name )
	: QWidget( parent, name )
{
	readOptions();

	QRadioButton *rb;
	QLabel *label;

	//CT 12Nov1998 layout management
	QGridLayout *lay = new QGridLayout(this,8,5,10,5);
	lay->addRowSpacing(0,10);
	lay->addRowSpacing(4,10);
	lay->addRowSpacing(0,10);
	lay->addRowSpacing(3,10);

	lay->setRowStretch(0,0);
	lay->setRowStretch(1,1);
	lay->setRowStretch(2,1);
	lay->setRowStretch(3,0);
	lay->setRowStretch(4,0);
	lay->setRowStretch(5,0);
	lay->setRowStretch(6,0);
	lay->setRowStretch(7,10);

	lay->setColStretch(0,0);
	lay->setColStretch(1,1);
	lay->setColStretch(2,2);
	lay->setColStretch(3,0);
	//CT

	QButtonGroup *bg = new QButtonGroup( klocale->translate("Font Size"), this );

	//CT 12Nov1998 layout management
	QGridLayout *bgLay = new QGridLayout(bg,2,3,10,5);
	bgLay->addRowSpacing(0,10);
	bgLay->setRowStretch(0,0);
	bgLay->setRowStretch(1,1);
	//CT

	bg->setExclusive( TRUE );
	//CT	bg->setGeometry( 15, 15, 300, 50 );

	rb = new QRadioButton( klocale->translate("Small"), bg );
	//CT	rb->setGeometry( 10, 20, 80, 20 );
	//CT 12Nov1998 layout management
	rb->adjustSize();
	rb->setMinimumSize(rb->size());
	bgLay->addWidget(rb,1,0);
	//CT
	rb->setChecked( fSize == 3 );

	rb = new QRadioButton( klocale->translate("Medium"), bg );
	//CT	rb->setGeometry( 100, 20, 80, 20 );
	//CT 12Nov1998 layout management
	rb->adjustSize();
	rb->setMinimumSize(rb->size());
	bgLay->addWidget(rb,1,1);
	//CT
	rb->setChecked( fSize == 4 );

	rb = new QRadioButton( klocale->translate("Large"), bg );
	//CT	rb->setGeometry( 200, 20, 80, 20 );
	//CT 12Nov1998 layout management
	rb->adjustSize();
	rb->setMinimumSize(rb->size());
	bgLay->addWidget(rb,1,2);
	//CT
	rb->setChecked( fSize == 5 );

	//CT 12Nov1998 layout management
	bgLay->activate();

	lay->addMultiCellWidget(bg,1,1,1,2);
	//CT

	label = new QLabel( klocale->translate("Standard Font"), this );
	//CT	label->setGeometry( 15, 90, 100, 20 );
	//CT 12Nov1998 layout management
	label->adjustSize();
	label->setMinimumSize(label->size());
	lay->addWidget(label,3,1);
	//CT

	QComboBox *cb = new QComboBox( false, this );
	//CT	cb->setGeometry( 120, 90, 180, 25 );
	getFontList( standardFonts, "-*-*-*-*-*-*-*-*-*-*-p-*-*-*" );
	cb->insertStrList( &standardFonts );
	QStrListIterator sit( standardFonts );
	int i;
	for ( i = 0; sit.current(); ++sit, i++ )
	{
		if ( !strcmp( stdName, sit.current() ) )
			cb->setCurrentItem( i );
	}

	//CT 12Nov1998 layout management
	cb->adjustSize();
	cb->setMinimumSize(cb->size());
	lay->addWidget(cb,3,2);
	//CT

	connect( cb, SIGNAL( activated( const char * ) ),
		SLOT( slotStandardFont( const char * ) ) );

	label = new QLabel( klocale->translate( "Fixed Font"), this );
	//CT	label->setGeometry( 15, 130, 100, 20 );
	//CT 12Nov1998 layout management
	label->adjustSize();
	label->setMinimumSize(label->size());
	lay->addWidget(label,4,1);
	//CT

	cb = new QComboBox( false, this );
	//CT	cb->setGeometry( 120, 130, 180, 25 );
	getFontList( fixedFonts, "-*-*-*-*-*-*-*-*-*-*-m-*-*-*" );
	cb->insertStrList( &fixedFonts );
	QStrListIterator fit( fixedFonts );
	for ( i = 0; fit.current(); ++fit, i++ )
	{
		if ( !strcmp( fixedName, fit.current() ) )
			cb->setCurrentItem( i );
	}
       
	//CT 12Nov1998 layout management
	cb->adjustSize();
	cb->setMinimumSize(cb->size());
	lay->addWidget(cb,4,2);

	connect( cb, SIGNAL( activated( const char * ) ),
		SLOT( slotFixedFont( const char * ) ) );


	// default charset Lars Knoll 17Nov98
	label = new QLabel( klocale->translate( "Default Charset"), this );
	label->adjustSize();
	label->setMinimumSize(label->size());
	lay->addWidget(label,5,1);
	lay->activate();

	cb = new QComboBox( false, this );
	QStrList charsets = kapp->getCharsets()->available();
	charsets.insert(0, klocale->translate("Use language charset"));
	cb->insertStrList( &charsets );
	QStrListIterator cs( charsets );
	for ( i = 0; cs.current(); ++cs, i++ )
	{
		if ( !strcmp( charsetName, cs.current() ) )
			cb->setCurrentItem( i );
	}

	cb->adjustSize();
	cb->setMinimumSize(cb->size());
	lay->addWidget(cb,5,2);

	connect( cb, SIGNAL( activated( const char * ) ),
		SLOT( slotCharset( const char * ) ) );

	connect( bg, SIGNAL( clicked( int ) ), SLOT( slotFontSize( int ) ) );
}


void KFontOptions::getFontOpts(struct fontoptions& fntopts){

  fntopts.fontsize     = fSize;
  fntopts.standardfont = stdName;
  fntopts.standardfont.detach();
  fntopts.fixedfont    = fixedName;
  fntopts.fixedfont.detach();
  fntopts.charset      = charsetName;
  fntopts.charset.detach();

  if(fontopts.fontsize != fSize || fontopts.standardfont != stdName ||
     fontopts.fixedfont != fixedName || fontopts.charset != charsetName){

     fntopts.changed     = true;
  }
  else
    fntopts.changed      = false;

}




void KFontOptions::readOptions()
{
	KConfig *config = KApplication::getKApplication()->getConfig();
	config->setGroup( "KFM HTML Defaults" );		
	QString fs = config->readEntry( "BaseFontSize" );
	if ( !fs.isEmpty() )
	{
		fSize = fs.toInt();
		if ( fSize < 3 )
			fSize = 3;
		else if ( fSize > 5 )
			fSize = 5;
	}
	else
		fSize = 3;
	fontopts.fontsize = fSize;

	stdName = config->readEntry( "StandardFont" );
	if ( stdName.isEmpty() )
		stdName = DEFAULT_VIEW_FONT;
	fontopts.standardfont = stdName;
	fontopts.standardfont.detach();

	fixedName = config->readEntry( "FixedFont" );
	if ( fixedName.isEmpty() )
		fixedName = DEFAULT_VIEW_FIXED_FONT;

	fontopts.fixedfont = fixedName;
	fontopts.fixedfont.detach();

	charsetName = config->readEntry( "DefaultCharset" );

	fontopts.charset = charsetName;
	fontopts.charset.detach();

}

void KFontOptions::getFontList( QStrList &list, const char *pattern )
{
	int num;

	char **xFonts = XListFonts( qt_xdisplay(), pattern, 2000, &num );

	for ( int i = 0; i < num; i++ )
	{
		addFont( list, xFonts[i] );
	}

	XFreeFontNames( xFonts );
}

void KFontOptions::addFont( QStrList &list, const char *xfont )
{


	const char *ptr = strchr( xfont, '-' );
	if ( !ptr )
		return;
	
	ptr = strchr( ptr + 1, '-' );
	if ( !ptr )
		return;

	QString font = ptr + 1;

	int pos;
	if ( ( pos = font.find( '-' ) ) > 0 )
	{
		font.truncate( pos );

		if ( font.find( "open look", 0, false ) >= 0 )
			return;

		QStrListIterator it( list );

		for ( ; it.current(); ++it )
			if ( it.current() == font )
				return;

		list.append( font );
	}
}


void KFontOptions::slotFontSize( int i )
{
	fSize = i+3;
}

void KFontOptions::slotStandardFont( const char *n )
{
	stdName = n;
}

void KFontOptions::slotFixedFont( const char *n )
{
	fixedName = n;
}

void KFontOptions::slotCharset( const char *n )
{
	charsetName = n;
}

//-----------------------------------------------------------------------------

KColorOptions::KColorOptions( QWidget *parent, const char *name )
    : QWidget( parent, name )
{
  	readOptions();

	KColorButton *colorBtn;
	QLabel *label;

	//CT 12Nov1998 layout management
	QGridLayout *lay = new QGridLayout(this,11,5,10,5);
	lay->addRowSpacing(0,10);
	lay->addRowSpacing(1,30);
	lay->addRowSpacing(2, 5);
	lay->addRowSpacing(3,30);
	lay->addRowSpacing(4, 5);
	lay->addRowSpacing(5,30);
	lay->addRowSpacing(6, 5);
	lay->addRowSpacing(7,30);
	lay->addRowSpacing(10,10);
	lay->addColSpacing(0,10);
	lay->addColSpacing(2,20);
	lay->addColSpacing(3,80);
	lay->addColSpacing(4,10);

	lay->setRowStretch(0,0);
	lay->setRowStretch(1,0);
	lay->setRowStretch(2,1);
	lay->setRowStretch(3,0);
	lay->setRowStretch(4,1);
	lay->setRowStretch(5,0);
	lay->setRowStretch(6,1);
	lay->setRowStretch(7,0);
	lay->setRowStretch(8,1);
	lay->setRowStretch(9,1);
	lay->setRowStretch(10,0);

	lay->setColStretch(0,0);
	lay->setColStretch(1,0);
	lay->setColStretch(2,1);
	lay->setColStretch(3,0);
	lay->setColStretch(4,1);
	//CT

	label = new QLabel( klocale->translate("Background Color:"), this );
	//CT	label->setGeometry( 35, 20, 165, 25 );
	//CT 12Nov1998 layout management
	label->adjustSize();
	label->setMinimumSize(label->size());
	lay->addWidget(label,1,1);
	//CT

	colorBtn = new KColorButton( bgColor, this );
	//CT	colorBtn->setGeometry( 200, 20, 80, 30 );
	//CT 12Nov1998 layout management
	colorBtn->adjustSize();
	colorBtn->setMinimumSize(colorBtn->size());
	lay->addWidget(colorBtn,1,3);
	//CT

	connect( colorBtn, SIGNAL( changed( const QColor & ) ),
		SLOT( slotBgColorChanged( const QColor & ) ) );

	label = new QLabel( klocale->translate("Normal Text Color:"), this );
	//CT	label->setGeometry( 35, 60, 165, 25 );
	//CT 12Nov1998 layout management
	label->adjustSize();
	label->setMinimumSize(label->size());
	lay->addWidget(label,3,1);
	//CT

	colorBtn = new KColorButton( textColor, this );
	//CT	colorBtn->setGeometry( 200, 60, 80, 30 );
	//CT 12Nov1998 layout management
	colorBtn->adjustSize();
	colorBtn->setMinimumSize(colorBtn->size());
	lay->addWidget(colorBtn,3,3);
	//CT

	connect( colorBtn, SIGNAL( changed( const QColor & ) ),
		SLOT( slotTextColorChanged( const QColor & ) ) );

	label = new QLabel( klocale->translate("URL Link Color:"), this );
	//CT	label->setGeometry( 35, 100, 165, 25 );
	//CT 12Nov1998 layout management
	label->adjustSize();
	label->setMinimumSize(label->size());
	lay->addWidget(label,5,1);
	//CT

	colorBtn = new KColorButton( linkColor, this );
	//CT	colorBtn->setGeometry( 200, 100, 80, 30 );
	//CT 12Nov1998 layout management
	colorBtn->adjustSize();
	colorBtn->setMinimumSize(colorBtn->size());
	lay->addWidget(colorBtn,5,3);
	//CT

	connect( colorBtn, SIGNAL( changed( const QColor & ) ),
		SLOT( slotLinkColorChanged( const QColor & ) ) );

	label = new QLabel( klocale->translate("Followed Link Color:"), this );
	//CT	label->setGeometry( 35, 140, 165, 25 );
	//CT 12Nov1998 layout management
	label->adjustSize();
	label->setMinimumSize(label->size());
	lay->addWidget(label,7,1);
	//CT

	colorBtn = new KColorButton( vLinkColor, this );
	//CT	colorBtn->setGeometry( 200, 140, 80, 30 );
	//CT 12Nov1998 layout management
	colorBtn->adjustSize();
	colorBtn->setMinimumSize(colorBtn->size());
	lay->addWidget(colorBtn,7,3);
	//CT

	connect( colorBtn, SIGNAL( changed( const QColor & ) ),
		SLOT( slotVLinkColorChanged( const QColor & ) ) );

	cursorbox = new QCheckBox(klocale->translate("Change cursor over link."),
				  this);

	//CT	cursorbox->setGeometry(35,180,250,28);
	//CT 12Nov1998 layout management
	cursorbox->adjustSize();
	cursorbox->setMinimumSize(cursorbox->size());
	lay->addMultiCellWidget(cursorbox,8,8,1,3);
	//CT
	cursorbox->setChecked(changeCursor);

	underlinebox = new QCheckBox(klocale->translate("Underline links"),
				  this);

	//CT	underlinebox->setGeometry(35,210,250,28);
	//CT 12Nov1998 layout management
	underlinebox->adjustSize();
	underlinebox->setMinimumSize(underlinebox->size());
	lay->addMultiCellWidget(underlinebox,9,9,1,3);
	//CT
	underlinebox->setChecked(underlineLinks);
}

void KColorOptions::readOptions()
{
	KConfig *config = KApplication::getKApplication()->getConfig();
	config->setGroup( "KFM HTML Defaults" );	
	bgColor = config->readColorEntry( "BgColor", &HTML_DEFAULT_BG_COLOR );
	textColor = config->readColorEntry( "TextColor", &HTML_DEFAULT_TXT_COLOR );
	linkColor = config->readColorEntry( "LinkColor", &HTML_DEFAULT_LNK_COLOR );
	vLinkColor = config->readColorEntry( "VLinkColor", &HTML_DEFAULT_VLNK_COLOR);
	changeCursor = (bool) config->readNumEntry("ChangeCursor",0);
	underlineLinks = (bool) config->readNumEntry("UnderlineLinks",1);
	changed = false;
}


void KColorOptions::slotBgColorChanged( const QColor &col )
{
	if ( bgColor != col )
	  bgColor = col;
	changed = true;

}

void KColorOptions::slotTextColorChanged( const QColor &col )
{
	if ( textColor != col )
	  textColor = col;
	changed = true;
}

void KColorOptions::slotLinkColorChanged( const QColor &col )
{
	if ( linkColor != col )
	  linkColor = col;
	changed = true;
}

void KColorOptions::slotVLinkColorChanged( const QColor &col )
{
	if ( vLinkColor != col )
	  vLinkColor = col;
	changed = true;
}

void KColorOptions::getColorOpts(struct coloroptions& coloropts){

  coloropts.bg      = bgColor;
  coloropts.text    = textColor;
  coloropts.link    = linkColor;
  coloropts.vlink   = vLinkColor;

  if((changeCursor != cursorbox->isChecked()) ||
     (underlineLinks != underlinebox->isChecked()))
    changed = true;

  coloropts.changeCursoroverLink = cursorbox->isChecked();
  coloropts.underlineLinks = underlinebox->isChecked();
  coloropts.changed = changed;
}

/************************************************************************/


KMiscOptions::KMiscOptions( QWidget *parent, const char *name )
    : QWidget( parent, name )
{
  	readOptions();

        //CT 12Nov1998
        //Sven: Inserted my checkbox in CT's layout
        
	QGridLayout *lay = new QGridLayout(this,10,5,10,5);
	lay->addRowSpacing(0,15);
	lay->addRowSpacing(1,30);
	lay->addRowSpacing(2, 5);
	lay->addRowSpacing(3,30);
	lay->addRowSpacing(6,30);
	lay->addRowSpacing(7, 5);
	lay->addRowSpacing(8,30);
	lay->addRowSpacing(9,10);
	lay->addColSpacing(0,10);
	lay->addColSpacing(2,10);
	lay->addColSpacing(3,80);
        lay->addColSpacing(4,35); //sven: colorbuttons are big

	lay->setRowStretch(0,0);
	lay->setRowStretch(1,0);
	lay->setRowStretch(2,1);
	lay->setRowStretch(3,0);
	lay->setRowStretch(4,1);
	lay->setRowStretch(5,1);
	lay->setRowStretch(6,0);
	lay->setRowStretch(7,1);
	lay->setRowStretch(8,0);
	lay->setRowStretch(9,0);

	lay->setColStretch(0,0);
	lay->setColStretch(1,0);
	lay->setColStretch(2,1);
	lay->setColStretch(3,0);
	lay->setColStretch(4,1);
	//CT
        
	QLabel *label;

	label = new QLabel( klocale->translate(
			    "Horizontal Root Grid Spacing:"), this );

	//CT	label->setGeometry( 35, 20, 180, 25 );
	//CT 12Nov1998
	label->adjustSize();
	label->setMinimumSize(label->size());
	lay->addWidget(label,1,1);
	//CT

	hspin  = new KNumericSpinBox(this);
	//CT	hspin ->setGeometry(225, 20, 40, 25 );
	//CT 12Nov1998
	hspin->adjustSize();
	hspin->setMinimumSize(hspin->size());
	lay->addWidget(hspin,1,3);
	//CT
	hspin->setRange(0,DEFAULT_GRID_MAX - DEFAULT_GRID_MIN);

	
	if(gridwidth - DEFAULT_GRID_MIN < 0 )
	  gridwidth = DEFAULT_GRID_MIN;
	hspin->setValue(gridwidth - DEFAULT_GRID_MIN);

	label = new QLabel( klocale->translate(
			    "Vertical Root Grid Spacing:"), this );

	//CT	label->setGeometry( 35, 60, 180, 25 );
	//CT 12Nov1998
	label->adjustSize();
	label->setMinimumSize(label->size());
	lay->addWidget(label,3,1);
	//CT

	vspin  = new KNumericSpinBox(this);
	//CT	vspin ->setGeometry(225, 60, 40, 25 );
	//CT 12Nov1998
	vspin->adjustSize();
	vspin->setMinimumSize(vspin->size());
	lay->addWidget(vspin,3,3);
	//CT
	vspin->setRange(0,DEFAULT_GRID_MAX - DEFAULT_GRID_MIN);

	if(gridheight - DEFAULT_GRID_MIN < 0 )
	  gridheight = DEFAULT_GRID_MIN;

	vspin->setValue(gridheight - DEFAULT_GRID_MIN);

        //----------------- sven---------
        urlpropsbox = new QCheckBox(klocale->translate("Allow per-URL settings"),
                                    this);
        urlpropsbox->adjustSize();
        urlpropsbox->setMinimumSize(urlpropsbox->size());
        lay->addMultiCellWidget(urlpropsbox,4,4,1,3);
	urlpropsbox->setChecked(urlprops);
        //-------------------------------

	iconstylebox = new QCheckBox(klocale->translate("Transparent Text for Root Icons."),
				  this);
	//CT	iconstylebox->setGeometry(35, 100, 280, 25);
	//CT 12Nov1998
	iconstylebox->adjustSize();
	iconstylebox->setMinimumSize(iconstylebox->size());
	lay->addMultiCellWidget(iconstylebox,5,5,1,3);
	//CT
	iconstylebox->setChecked(transparent);

        
	connect(iconstylebox,SIGNAL(toggled(bool)),this,SLOT(makeBgActive(bool)));

	//CT 12Nov1998 color buttons
	label = new QLabel(klocale->translate("Icon foreground color:"),this);
	label->adjustSize();
	label->setMinimumSize(label->size());
	lay->addWidget(label,6,1);

	fgColorBtn = new KColorButton(icon_fg,this);
	fgColorBtn->adjustSize();
	fgColorBtn->setMinimumSize(fgColorBtn->size());
	lay->addWidget(fgColorBtn,6,3);
	connect( fgColorBtn, SIGNAL( changed( const QColor & ) ),
		SLOT( slotIconFgColorChanged( const QColor & ) ) );

	bgLabel = new QLabel(klocale->translate("Icon background color:"),this);
	bgLabel->adjustSize();
	bgLabel->setMinimumSize(bgLabel->size());
	lay->addWidget(bgLabel,8,1);

	bgColorBtn = new KColorButton(icon_bg,this);
	bgColorBtn->adjustSize();
	bgColorBtn->setMinimumSize(bgColorBtn->size());
	lay->addWidget(bgColorBtn,8,3);
	connect( bgColorBtn, SIGNAL( changed( const QColor & ) ),
		SLOT( slotIconBgColorChanged( const QColor & ) ) );

	makeBgActive(transparent);

	lay->activate();
	//CT
}

void KMiscOptions::readOptions()
{
	KConfig *config = KApplication::getKApplication()->getConfig();
	config->setGroup( "KFM Misc Defaults" );	
	gridwidth = config->readNumEntry( "GridWidth", DEFAULT_GRID_WIDTH );
	gridheight = config->readNumEntry( "GridHeight", DEFAULT_GRID_HEIGHT );
        urlprops = config->readBoolEntry( "EnablePerURLProps", 0);
        config->setGroup( "KFM Root Icons" );	
	transparent = (bool)config->readNumEntry("Style", DEFAULT_ROOT_ICONS_STYLE);
	//CT 12Nov1998
	icon_fg = config->readColorEntry("Foreground",&DEFAULT_ICON_FG);
	icon_bg = config->readColorEntry("Background",&DEFAULT_ICON_BG);
	//CT

	changed = false;
}



void KMiscOptions::getMiscOpts(struct rootoptions& rootopts)
{
  if(gridwidth != (hspin->getValue()  + DEFAULT_GRID_MIN) || 
     gridheight != (vspin->getValue() + DEFAULT_GRID_MIN) )
  {
    changed = true;
  }
  if(transparent != iconstylebox->isChecked())
  {
    changed = true;
  }
  if(urlprops != urlpropsbox->isChecked())
  {
    changed = true;
  }

  rootopts.gridwidth = hspin->getValue()+DEFAULT_GRID_MIN;
  rootopts.gridheight = vspin->getValue()+DEFAULT_GRID_MIN;

  if (iconstylebox->isChecked())
    rootopts.iconstyle = 1;
  else
    rootopts.iconstyle = 0;

  if (urlpropsbox->isChecked())
    rootopts.urlprops = 1;
  else
    rootopts.urlprops = 0;

  rootopts.changed = changed;

  //CT 12Nov1998
  rootopts.icon_fg = icon_fg;
  rootopts.icon_bg = icon_bg;
  //CT
}

//CT 12Nov1998
void KMiscOptions::slotIconFgColorChanged(const QColor &col) {
	if ( icon_fg != col )
	  icon_fg = col;
	changed = true;
}

void KMiscOptions::slotIconBgColorChanged(const QColor &col) {
	if ( icon_bg != col )
	  icon_bg = col;
	changed = true;
}

void KMiscOptions::makeBgActive(bool a) {
  bgColorBtn->setEnabled(!a);
  bgLabel->setEnabled(!a);
}
  
//CT