/* -------------------------------------------------------------

   toplevel.h (part of Klipper - Cut & paste history for KDE)

   (C) by Andrew Stanley-Jones

   Generated with the KDE Application Generator

 ------------------------------------------------------------- */


#ifndef _TOPLEVEL_H_
#define _TOPLEVEL_H_

#include <kapp.h>
#include <kglobalaccel.h>
#include <ktmainwindow.h>
#include <qpopmenu.h>
#include <qclipboard.h>
#include <qintdict.h>
#include <qtimer.h>
#include <qpixmap.h>

class TopLevel : public KTMainWindow
{
  Q_OBJECT

public:
    TopLevel();
    ~TopLevel();

    KGlobalAccel *globalKeys;

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

protected slots:
    void globalKeyEvent() { mousePressEvent( 0L ); }
  
private slots:
    void newClipData();
    void clickedMenu(int);

private:
    QString QSlast;
    QPopupMenu *pQPMmenu;
    QIntDict<QString> *pQIDclipData;
    QTimer *pQTcheck;
    QPixmap *pQPpic;
};

#endif

