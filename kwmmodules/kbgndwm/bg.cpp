/*
 * bg.cpp.  Part of the KDE Project.
 *
 * Copyright (C) 1997 Martin Jones
 *
 * Gradient fill added by Mark Donohoe 1997
 *
 */

//----------------------------------------------------------------------------

#include <stdlib.h>

#include <qimage.h>
#include <qfile.h>
#include <qpainter.h>
#include <qstring.h>
#include <qpmcache.h>

#include <kapp.h>
#include <dither.h>
#include <ksimpleconfig.h>

#include "bg.h"
#include "bg.moc"

//----------------------------------------------------------------------------

#define NO_WALLPAPER	"(none)"

//----------------------------------------------------------------------------

class GPixmap : public QPixmap
{
public:
    GPixmap();
    void gradientFill(QColor color1, QColor color2, bool updown = TRUE, 
		      int num_colors = 8);
private:
    QPixmap cropped_pm;
    QPixmap full_pm;
    int x_size;
    int y_size;
    QColor col;
    int h, s;
    int red1, green1, blue1;
    int red_dif, green_dif, blue_dif;
    float rat;
    uint *p;
    uint rgbcol;
    int steps;
};

GPixmap::GPixmap()
{}

void GPixmap::gradientFill(QColor color1, QColor color2, bool updown, int num_colors)
{    
    if(updown)
    	y_size=height();
    else
    	y_size=width();
        
    cropped_pm.resize( 30, y_size );
 
    QImage image(30, y_size, 32);
	

    red1 = color1.red();
    green1 = color1.green();
    blue1 = color1.blue();
	
    red_dif = color2.red()-color1.red();
    green_dif = color2.green()-color1.green();
    blue_dif = color2.blue()-color1.blue();

    for ( s = y_size-1; s > 0; s-- )
    {
	p = (uint *) image.scanLine( y_size - s - 1 );
	rat=1.0*s/y_size;
	col.setRgb( red1+(int)(red_dif*rat),
	    green1+(int)(green_dif*rat), blue1+(int)(blue_dif*rat) );
	rgbcol= col.rgb();

	for( h = 0; h < 30; h++ ) {
	    *p = rgbcol;
	    p++;
	}
    }
	
    if(num_colors<2 || num_colors >256) {
	num_colors=8;
    }
	
    QColor *ditherPalette = new QColor[num_colors];
    for( s=0; s<num_colors; s++) {
	ditherPalette[s].setRgb(red1+red_dif*s/(num_colors-1),
	green1+green_dif*s/(num_colors-1),
	blue1+blue_dif*s/(num_colors-1) );
    }
	
    kFSDither dither( ditherPalette, num_colors );
    QImage tImage = dither.dither( image );
    cropped_pm.convertFromImage( tImage );
	
    if(updown)
	steps=width()/20+1;
    else
	steps=height()/20+1;
	
    QPainter p;
    p.begin( this );
    if(updown)	
	for(s=0;s<steps;s++)
	    p.drawPixmap(20*s, 0,cropped_pm, 5, 0 , 20, y_size );
    else {
	QWMatrix matrix;
        matrix.translate( (float)width()-1.0, 0.0);
       	matrix.rotate( 90.0 );
       	p.setWorldMatrix( matrix );
       	for(s=0;s<steps;s++)
	p.drawPixmap(20*s, 0,cropped_pm, 5, 0 , 20, y_size );
    }
    p.end();

    delete [] ditherPalette;
}	 

//----------------------------------------------------------------------------

KBackground::KBackground()
{
    wpMode = Tiled;
    gfMode = Flat;
    orMode = Portrait;

    bgPixmap = 0;
    applied = false;

    hasPm = false;
}

KBackground::~KBackground()
{
}

/*
 * Read the settings from kdisplayrc
 */
void KBackground::readSettings( const char *group )
{
    QString cFile;

    hasPm = false;
    name = group;

    cFile = getenv( "HOME" );
    cFile += "/.kde/share/config/kdisplayrc";

    KSimpleConfig config( cFile, true );

    config.setGroup( group );

    QString str;

    str = config.readEntry( "Color1", "#CCCCCC" );
    color1.setNamedColor( str );

    str = config.readEntry( "Color2", "#CCCCCC" );
    color2.setNamedColor( str );

    gfMode=Flat;
    str = config.readEntry( "ColorMode" );
    if ( !str.isEmpty() && str == "Gradient" )
    {
	gfMode = Gradient;
	hasPm = true;
    }

    if (str == "Pattern") {
	gfMode = Pattern;
	QStrList strl;
	config.readListEntry("Pattern", strl);
	uint size = strl.count();
	if (size > 8) size = 8;
	uint i = 0;
	for (i = 0; i < 8; i++)
	    pattern[i] = (i < size) ? QString(strl.at(i)).toUInt() : 255;
    }

    orMode=Portrait;
    str = config.readEntry( "OrientationMode" );
    if ( !str.isEmpty() && str == "Landscape" )
	orMode = Landscape;

    wpMode = Tiled;
    str = config.readEntry( "WallpaperMode" );
    if ( !str.isEmpty() )
    {
	if ( str == "Centred" )
	    wpMode = Centred;
	else if ( str == "Scaled" )
	    wpMode = Scaled;
    }

    wallpaper = NO_WALLPAPER;
    str = config.readEntry( "Wallpaper" );
    if ( !str.isEmpty() )
	wallpaper = str;

    name.sprintf( "%s_%d_%d_%d#%02x%02x%02x#%02x%02x%02x#", wallpaper.data(), 
		  wpMode, gfMode, orMode, color1.red(), color1.green(), 
		  color1.blue(), color1.red(), color2.green(), color2.blue());
    
    QString tmp;
    for (int i = 0; i < 8; i++) {
	tmp.sprintf("%02x", pattern[i]);
	name += tmp;
    }

    hasPm = true;

}

QPixmap *KBackground::loadWallpaper()
{
    if ( wallpaper == NO_WALLPAPER )
	return 0;

    QString filename;

    if ( wallpaper[0] != '/' )
    {
	filename = KApplication::kde_wallpaperdir().copy() + "/";
	filename += wallpaper;
    }
    else
	filename = wallpaper;

    QPixmap *wpPixmap = new QPixmap;

    if ( wpPixmap->load( filename ) == FALSE )
    {
	delete wpPixmap;
	wpPixmap = 0;
    }

    return wpPixmap;
}

void KBackground::apply()
{
    applied = false;

    // the background pixmap is cached?
    bgPixmap = QPixmapCache::find( name );
    if ( bgPixmap )
    {
	debug( "Desktop background found in cache" );
	qApp->desktop()->setBackgroundPixmap( *bgPixmap );
	bgPixmap = 0;
	applied = true;
	return;
    }

    QPixmap *wpPixmap = loadWallpaper();
    
    uint w, h;

    if (wpPixmap) {
	w = QApplication::desktop()->width();
	h = QApplication::desktop()->height();

	bgPixmap = new QPixmap;

    }

    if ( !wpPixmap || (wpMode == Centred) ) {
	if (bgPixmap)
	    bgPixmap->resize(w, h);
	
	switch (gfMode) {

	case Gradient:
	    {
		int numColors = 8;
		if ( QColor::numBitPlanes() > 8 )
		    numColors = 16;
		
		GPixmap pmDesktop;
		
		if ( orMode == Portrait ) {

		    pmDesktop.resize( 15, QApplication::desktop()->height() );
		    pmDesktop.gradientFill( color2, color1, true, numColors );

		} else {

		    pmDesktop.resize( QApplication::desktop()->width(), 20 );
		    pmDesktop.gradientFill( color2, color1, false, numColors );
		    
		}

		delete bgPixmap;
		bgPixmap = new QPixmap();
		
		if (! wpPixmap ) {

		    qApp->desktop()->setBackgroundPixmap(pmDesktop);
		    *bgPixmap = pmDesktop;
		    
		} else {
		    bgPixmap->resize(w, h);
		    
		    if ( orMode == Portrait ) {
			for (uint pw = 0; pw <= w; pw += pmDesktop.width())
			    bitBlt( bgPixmap, pw, 0, &pmDesktop, 0, 0, 
				    pmDesktop.width(), h);
		    } else {
			for (uint ph = 0; ph <= h; ph += pmDesktop.height()) {
			    debug("land %d",ph);
			    bitBlt( bgPixmap, 0, ph, &pmDesktop, 0, 0, 
				    w, pmDesktop.height());
			}
		    }
		}

		// background switch is deferred in case the user switches
		// again while the background is loading
		startTimer( 0 );
	    }
	    break;
	    
	case Flat:
	    if (wpPixmap ) {
		delete bgPixmap;
		bgPixmap = new QPixmap(w, h);
		bgPixmap->fill( color1 );
	    } else {
		qApp->desktop()->setBackgroundColor( color1 );
		applied = true;
	    }
	    break;
	    
	case Pattern: 
	    {
		QPixmap tile(8, 8);
		tile.fill(color2);
		QPainter pt;
		pt.begin(&tile);
		pt.setBackgroundColor( color2 );
		pt.setPen( color1 );
		
		for (int y = 0; y < 8; y++) {
		    uint v = pattern[y];
		    for (int x = 0; x < 8; x++) {
			if ( v & 1 )
			    pt.drawPoint(7 - x, y);
			v /= 2;
		    }
		}
		pt.end();

		delete bgPixmap;
		bgPixmap = new QPixmap();

		if (! wpPixmap ) {
		    qApp->desktop()->setBackgroundPixmap(tile);
		    *bgPixmap = tile;
		    applied = true;
		} else {
		    bgPixmap->resize(w, h);
		    uint sx, sy = 0;
		    while (sy < h) {
			sx = 0;
			while (sx < w) {
			    bitBlt( bgPixmap, sx, sy, &tile, 0, 0, 8, 8);
			    sx += 8;
			}
			sy += 8;
		    }
		}
		break;
	    }
	}
	
    }
    
    if ( wpPixmap )
	{
	    
	    switch ( wpMode )
		{
		case Centred:
		    {
			bitBlt( bgPixmap, ( w - wpPixmap->width() ) / 2,
				( h - wpPixmap->height() ) / 2, wpPixmap, 0, 0,
				wpPixmap->width(), wpPixmap->height() );
		    }
		    break;
		    
		case Scaled:
		    {
			float sx = (float)w / wpPixmap->width();
			float sy = (float)h / wpPixmap->height();
			QWMatrix matrix;
			matrix.scale( sx, sy );
			*bgPixmap = wpPixmap->xForm( matrix );
		    }
		    break;
		    
		case Tiled:
		    *bgPixmap = *wpPixmap;
		}
	    
	    delete wpPixmap;
	    wpPixmap = 0;
	    
	    // background switch is deferred in case the user switches
	    // again while the background is loading
	    startTimer( 0 );
	}
}

void KBackground::cancel()
{
    killTimers();
    if ( bgPixmap )
	delete bgPixmap;
}

void KBackground::timerEvent( QTimerEvent * )
{
    killTimers();

    if ( !bgPixmap )
	return;

    qApp->desktop()->setBackgroundPixmap( *bgPixmap );
    delete bgPixmap;
    bgPixmap = 0;
    applied = true;
}

//----------------------------------------------------------------------------

