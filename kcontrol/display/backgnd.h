//-----------------------------------------------------------------------------
//
// KDE Display background setup module
//
// Copyright (c)  Martin R. Jones 1996
//

#ifndef __BACKGND_H__
#define __BACKGND_H__

#include <qwidget.h>
#include <qpushbt.h>
#include <qcombo.h>
#include <qlistbox.h>
#include <qradiobt.h>
#include <qlined.h>
#include <qbttngrp.h>
#include "kcolordlg.h"
#include "kcolorbtn.h"
#include <kcontrol.h>
#include "display.h"

class KBGMonitor : public QWidget
{
	Q_OBJECT
public:
	KBGMonitor( QWidget *parent ) : QWidget( parent ) {};

	// we don't want no steenking palette change
	virtual void setPalette( const QPalette & ) {};
};

class KRenameDeskDlg : public QDialog
{
    Q_OBJECT
public:
    KRenameDeskDlg( const char *t, QWidget *parent );
    virtual ~KRenameDeskDlg() {}

    const char *title()
	{  return edit->text(); }

private:
    QLineEdit *edit;
};

class PatternEntry 
{
public: 
    PatternEntry() {}
    PatternEntry( QString n, uint p[]) { 
	name = n;
	for (uint i = 0; i < 8; i++)
	    pattern[i] = p[i];
    }

    QString name;
    uint pattern[8];

    operator==( uint p[] ) {
	for (uint i = 0; i < 8; i++)
	    if (pattern[i] != p[i]) 
		return false;
	return true;
    }

    operator==( const char *item) {
	return name == item;
    }
	
};

class KBPatternDlg : public QDialog
{
    Q_OBJECT

public:
    KBPatternDlg( QColor color1, QColor color2, uint p[], 
		  QWidget *parent = 0, char *name = 0 );
    
protected:
    int savePatterns();
   
protected slots:
    void selected(const char *item);
    virtual void done ( int r );
    
private:
    QLabel *preview;
    QListBox *listBox;
    QList<PatternEntry> list;

    bool changed;
    PatternEntry *current;
    uint *pattern;
    
    QColor color1, color2;
};

class KBackground : public KDisplayModule
{
	Q_OBJECT
public:
	KBackground( QWidget *parent, int mode, int desktop = 0 );

	virtual void readSettings( int deskNum = 0 );
	virtual void apply( bool force = FALSE );

        virtual void loadSettings();
        virtual void applySettings();

protected slots:
	void slotApply();
	void slotSelectColor1( const QColor &col );
	void slotSelectColor2( const QColor &col );
	void slotBrowse();
	void slotWallpaper( const char * );
	void slotWallpaperMode( int );
	void slotGradientMode( int );
	void slotOrientMode( int );
	void slotSwitchDesk( int );
	void slotRenameDesk();
	void slotHelp();
        void slotChangePattern();

protected:
	void getDeskNameList();
	void setDesktop( int );
	void showSettings();
	void writeSettings( int deskNum = 0 );
	void setMonitor();
	int  loadWallpaper( const char *filename, bool useContext = TRUE );
	void retainResources();

protected:
	enum { Tiled = 1, Centred, Scaled };
	enum { Flat = 1, Gradient, Pattern };
	enum { Portrait = 1, Landscape };

	QListBox     *deskListBox;
	QRadioButton *rbPortrait;
	QRadioButton *rbLandscape;
	KColorButton *colButton1;
	KColorButton *colButton2;
	QButtonGroup *gfGroup;
	QButtonGroup *orGroup;
	QButtonGroup *wpGroup;
        QPushButton  *changeButton;
	KBGMonitor* monitor;
	QComboBox *wpCombo;
	QColor color1;
	QColor color2;
	QString wallpaper;
	QPixmap wpPixmap;
	QString deskName;
	QStrList deskNames;
	int wpMode;
	int gfMode;
	int orMode;
	int deskNum;
	int maxDesks;

        uint pattern[8];

	bool changed;
};


#endif

