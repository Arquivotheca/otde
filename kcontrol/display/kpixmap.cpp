
//
// KPixmap
//
// Copyright (c)  Mark Donohoe 1998
//
// Pattern fill by Stephan Kulow 1998
//

#include <qpixmap.h>
#include <qpainter.h>
#include <qimage.h>
#include <qbitmap.h>
#include <qcolor.h>

#include <kapp.h>
#include <dither.h>

#include "kpixmap.h"

// Fast diffuse dither to 3x3x3 color cube
// Based on Qt's image conversion functions

static bool kdither_32_to_8( const QImage *src, QImage *dst )
{
    register QRgb *p;
    uchar  *b;
    int	    y;
	
	//printf("kconvert_32_to_8\n");
	
    if ( !dst->create(src->width(), src->height(), 8, 256) ) {
		printf("kconvert - Destination not valid\n");
		return FALSE;
	}

	printf("kconvert32_to8 - Will quantize this image \n");
    int ncols = 256;

    static uint bm[16][16];
    static int init=0;
    if (!init) {
		// Build a Bayer Matrix for dithering

		init = 1;
		int n, i, j;

		bm[0][0]=0;

		for (n=1; n<16; n*=2) {
	    	for (i=0; i<n; i++) {
			for (j=0; j<n; j++) {
		    	bm[i][j]*=4;
		    	bm[i+n][j]=bm[i][j]+2;
		    	bm[i][j+n]=bm[i][j]+3;
		    	bm[i+n][j+n]=bm[i][j]+1;
			}
	    	}
		}

		for (i=0; i<16; i++)
	    	for (j=0; j<16; j++)
			bm[i][j]<<=8;
    }

    dst->setNumColors( ncols );

#define MAX_R 2
#define MAX_G 2
#define MAX_B 2
#define INDEXOF(r,g,b) (((r)*(MAX_G+1)+(g))*(MAX_B+1)+(b))

	int rc, gc, bc;

	for ( rc=0; rc<=MAX_R; rc++ )		// build 2x2x2 color cube
	    for ( gc=0; gc<=MAX_G; gc++ )
		for ( bc=0; bc<=MAX_B; bc++ ) {
		    dst->setColor( INDEXOF(rc,gc,bc),
			qRgb( rc*255/MAX_R, gc*255/MAX_G, bc*255/MAX_B ) );
		}	

	int sw = src->width();
	int* line1[3];
	int* line2[3];
	int* pv[3];

	line1[0] = new int[src->width()];
	line2[0] = new int[src->width()];
	line1[1] = new int[src->width()];
	line2[1] = new int[src->width()];
	line1[2] = new int[src->width()];
	line2[2] = new int[src->width()];
	pv[0] = new int[sw];
	pv[1] = new int[sw];
	pv[2] = new int[sw];

	for ( y=0; y < src->height(); y++ ) {
	    p = (QRgb *)src->scanLine(y);
	    b = dst->scanLine(y);
		int endian = (QImage::systemByteOrder() == QImage::BigEndian);
		int x;
		uchar* q = src->scanLine(y);
		uchar* q2 = src->scanLine(y+1 < src->height() ? y + 1 : 0);
		for (int chan = 0; chan < 3; chan++) {
		    b = dst->scanLine(y);
		    int *l1 = (y&1) ? line2[chan] : line1[chan];
		    int *l2 = (y&1) ? line1[chan] : line2[chan];
		    if ( y == 0 ) {
			for (int i=0; i<sw; i++)
			    l1[i] = q[i*4+chan+endian];
		    }
		    if ( y+1 < src->height() ) {
			for (int i=0; i<sw; i++)
			    l2[i] = q2[i*4+chan+endian];
		    }
		    // Bi-directional error diffusion
		    if ( y&1 ) {
			for (x=0; x<sw; x++) {
			    int pix = QMAX(QMIN(2, (l1[x] * 2 + 128)/ 255), 0);
			    int err = l1[x] - pix * 255 / 2;
			    pv[chan][x] = pix;

			    // Spread the error around...
			    if ( x+1<sw ) {
				l1[x+1] += (err*7)>>4;
				l2[x+1] += err>>4;
			    }
			    l2[x]+=(err*5)>>4;
			    if (x>1) 
				l2[x-1]+=(err*3)>>4;
			}
		    } else {
			for (x=sw; x-->0; ) {
			    int pix = QMAX(QMIN(2, (l1[x] * 2 + 128)/ 255), 0);
			    int err = l1[x] - pix * 255 / 2;
			    pv[chan][x] = pix;

			    // Spread the error around...
			    if ( x > 0 ) {
				l1[x-1] += (err*7)>>4;
				l2[x-1] += err>>4;
			    }
			    l2[x]+=(err*5)>>4;
			    if (x+1 < sw) 
				l2[x+1]+=(err*3)>>4;
			}
		    }
		}
		if (endian) {
		    for (x=0; x<sw; x++) {
			*b++ = INDEXOF(pv[2][x],pv[1][x],pv[0][x]);
		    }
		} else {
		    for (x=0; x<sw; x++) {
			*b++ = INDEXOF(pv[0][x],pv[1][x],pv[2][x]);
		    }
		}
	}

	delete line1[0];
	delete line2[0];
	delete line1[1];
	delete line2[1];
	delete line1[2];
	delete line2[2];
	delete pv[0];
	delete pv[1];
	delete pv[2];
	
#undef MAX_R
#undef MAX_G
#undef MAX_B
#undef INDEXOF

    return TRUE;
}

void KPixmap::gradientFill( QColor ca, QColor cb, bool upDown, int ncols )
{				
	QPixmap pmCrop;
	QColor cRow;
    int ySize;
    int rca, gca, bca;
    int rDiff, gDiff, bDiff;
    float rat;
    uint *p;
    uint rgbRow;
	
	
	if( upDown )
    	ySize = height();
    else
    	ySize = width();
    
    pmCrop.resize( 30, ySize );
    QImage image( 30, ySize, 32 );
    
    rca = ca.red();
    gca = ca.green();
    bca = ca.blue();
    rDiff = cb.red() - ca.red();
    gDiff = cb.green() - ca.green();
    bDiff = cb.blue() - ca.blue();
    
    for ( int y = ySize - 1; y > 0; y-- ) {
	    p = (uint *) image.scanLine( ySize - y - 1 );
	    rat = 1.0 * y / ySize;
	    
		cRow.setRgb( rca + (int) ( rDiff * rat ),
						gca + (int) ( gDiff * rat ), 
						bca + (int) ( bDiff * rat ) );
	    
		rgbRow = cRow.rgb();
	    
	    for( int x = 0; x < 30; x++ ) {
			*p = rgbRow;
			p++;
	    }
	}
	
	if ( QColor::numBitPlanes() == 8 ) {
	
		if ( ncols < 2 || ncols > 256 ) ncols = 3;

		QColor *dPal = new QColor[ncols];
		for ( int i=0; i<ncols; i++) {
			dPal[i].setRgb ( rca + rDiff * i / ( ncols - 1 ),
								gca + gDiff * i / ( ncols - 1 ),
								bca + bDiff * i / ( ncols - 1 ) );
		}

		kFSDither dither( dPal, ncols );
		QImage dImage = dither.dither( image );
		pmCrop.convertFromImage( dImage );
		
		delete [] dPal;	
		
	} else 
		pmCrop.convertFromImage( image );
	
	// Copy the cropped pixmap into the KPixmap.
	// Extract only a central column from the cropped pixmap
	// to avoid edge effects.
	
	int s;
	int sSize = 20;
	int sOffset = 5;
	
	if( upDown )
	    s = width() / sSize + 1;
	else
	    s = height() / sSize + 1;
	
	QPainter paint;
	paint.begin( this );
	
	if ( upDown )	
	    for( int i=0; i<s; i++ )
			paint.drawPixmap( sSize*i, 0, pmCrop, sOffset, 0 , sSize, ySize );
	else {
	    QWMatrix matrix;
	    matrix.translate( (float) width() - 1.0, 0.0 );
	    matrix.rotate( 90.0 );
	    paint.setWorldMatrix( matrix );
	    for( int i=0; i<s; i++)
			paint.drawPixmap( sSize*i, 0, pmCrop, sOffset, 0 , sSize, ySize );
	}
	
	paint.end();
}

void KPixmap::patternFill( QColor ca, QColor cb, uint pattern[8] )
{
    QPixmap tile( 8, 8 );
    tile.fill( cb );
	
    QPainter pt;
    pt.begin( &tile );
    pt.setBackgroundColor( cb );
    pt.setPen( ca );
    
    for ( int y = 0; y < 8; y++ ) {
		uint v = pattern[y];
		for ( int x = 0; x < 8; x++ ) {
	    	if ( v & 1 )
				pt.drawPoint( 7 - x, y );
	    	v /= 2;
		}
    }
    
    pt.end();

    int sx, sy = 0;
    while ( sy < height() ) {
		sx = 0;
		while (sx < width()) {
	    	bitBlt( this, sx, sy, &tile, 0, 0, 8, 8 );
	    	sx += 8;
		}
		sy += 8;
    }
}

bool KPixmap::load( const char *fileName, const char *format,
		    int conversion_flags )
{
    QImageIO io( fileName, format );

    bool result = io.read();
	
    if ( result ) {
	detach();
	result = convertFromImage( io.image(), conversion_flags );
    }
    return result;
}

bool KPixmap::load( const char *fileName, const char *format,
		    ColorMode mode )
{
    int conversion_flags = 0;
    switch (mode) {
      case Color:
	conversion_flags |= ColorOnly;
	break;
      case Mono:
	conversion_flags |= MonoOnly;
	break;
		case LowColor:
	conversion_flags |= LowOnly;
	break;
	  case WebColor:
	conversion_flags |= WebOnly;
	break;
      default:
	;// Nothing.
    }
    return load( fileName, format, conversion_flags );
}

bool KPixmap::convertFromImage( const QImage &img, ColorMode mode )
{
int conversion_flags = 0;
    switch (mode) {
      case Color:
	conversion_flags |= ColorOnly;
	break;
      case Mono:
	conversion_flags |= MonoOnly;
	break;
		case LowColor:
	conversion_flags |= LowOnly;
	break;
	  case WebColor:
	conversion_flags |= WebOnly;
	break;
      default:
	;	// Nothing.
    }
    return convertFromImage( img, conversion_flags );
}

bool KPixmap::convertFromImage( const QImage &img, int conversion_flags  )
{
	if ( img.isNull() ) {
#if defined(CHECK_NULL)
	warning( "KPixmap::convertFromImage: Cannot convert a null image" );
#endif
	return FALSE;
    }
    detach();					// detach other references
    
	if( ( conversion_flags != LowOnly && conversion_flags != WebOnly ) 
		||  QColor::numBitPlanes() > 8 ) {
		return QPixmap::convertFromImage ( img, conversion_flags );
	}
		
	if( conversion_flags == LowOnly ) {
		
		// If image uses icon palette don't dither it.
		if( img.numColors() > 0 && img.numColors() <=40 ) {
			if ( checkColorTable( img ) ) {
				return QPixmap::convertFromImage( img, QPixmap::Auto );
			}
		}
		
		QImage  image = img.convertDepth(32);
		
		QImage tImage( image.width(), image.height(), 8, 256 );
		kdither_32_to_8( &image, &tImage );
		
		return QPixmap::convertFromImage ( tImage);
	} else {
		QImage  image = img.convertDepth(32);
		return QPixmap::convertFromImage ( image, conversion_flags );
	}
}

bool KPixmap::checkColorTable( const QImage &image ) 
{
	QColor *iconPalette = new QColor[40];
	int i = 0;
	
	int ncols = image.numColors();

	// Standard palette
    iconPalette[i++] = red;
    iconPalette[i++] = green;
    iconPalette[i++] = blue;
    iconPalette[i++] = cyan;
    iconPalette[i++] = magenta;
    iconPalette[i++] = yellow;
    iconPalette[i++] = darkRed;
    iconPalette[i++] = darkGreen;
    iconPalette[i++] = darkBlue;
    iconPalette[i++] = darkCyan;
    iconPalette[i++] = darkMagenta;
    iconPalette[i++] = darkYellow;
    iconPalette[i++] = white;
    iconPalette[i++] = lightGray;
    iconPalette[i++] = gray;
    iconPalette[i++] = darkGray;
    iconPalette[i++] = black;

	// Pastels
	iconPalette[i++] = QColor( 255, 192, 192 );
	iconPalette[i++] = QColor( 192, 255, 192 );
	iconPalette[i++] = QColor( 192, 192, 255 );
	iconPalette[i++] = QColor( 255, 255, 192 );
	iconPalette[i++] = QColor( 255, 192, 255 );
	iconPalette[i++] = QColor( 192, 255, 255 );

	// Reds
	iconPalette[i++] = QColor( 64,   0,   0 );
	iconPalette[i++] = QColor( 192,  0,   0 );

	// Oranges
	iconPalette[i++] = QColor( 255, 128,   0 );
	iconPalette[i++] = QColor( 192,  88,   0 );
	iconPalette[i++] = QColor( 255, 168,  88 );
	iconPalette[i++] = QColor( 255, 220, 168 );

	// Blues
	iconPalette[i++] = QColor(   0,   0, 192 );

	// Turquoise
	iconPalette[i++] = QColor(   0,  64,  64 );
	iconPalette[i++] = QColor(   0, 192, 192 );

	// Yellows
	iconPalette[i++] = QColor(  64,  64,   0 );
	iconPalette[i++] = QColor( 192, 192,   0 );

	// Greens
	iconPalette[i++] = QColor(   0,  64,   0 );
	iconPalette[i++] = QColor(   0, 192,   0 );

	// Purples
	iconPalette[i++] = QColor( 192,   0, 192 );

	// Greys
	iconPalette[i++] = QColor(  88,  88,  88 );
	iconPalette[i++] = QColor(  48,  48,  48 );
	iconPalette[i++] = QColor( 220, 220, 220 );

	int j;
	QRgb* ctable = image.colorTable();
	
	// Allow one failure = transparent background
	int failures = 0;
	
	for ( i=0; i<ncols; i++ ) {
		for ( j=0; j<40; j++ ) {
			if ( iconPalette[j].red() == qRed( ctable[i] ) &&
				 iconPalette[j].green() == qGreen( ctable[i] ) &&
				 iconPalette[j].blue() == qBlue( ctable[i] ) ) {
				break;
			}
		}

		if ( j == 40 ) {
			failures ++;			
		}
	}
	
	if( failures > 1 )
		return FALSE;
	else
		return TRUE;
}
