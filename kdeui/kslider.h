/* This file is part of the KDE libraries
    Copyright (C) 1997 Martin Jones (mjones@kde.org)
              (C) 1997 Christian Esken (esken@kde.org)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
*/
//-----------------------------------------------------------------------------
// Slider control V2.0
//
// V1.0 Copyright (C) Martin R. Jones 1997
// V1.1 and following: (C) 1997 Christian Esken (esken@kde.org)

#ifndef __KSLIDER_H__
#define __KSLIDER_H__

// undef Above+Below because of Qt <-> X11 collision. Grr, I hate X11 headers
#undef Above
#undef Below
#include <qslider.h>

/// Control for selecting a value from using a sliding pointer.
/**
KSlider is a control for selecting a value from using a sliding pointer.
It can be setup similarly to a scrollbar
Calling setSteps( line, page ) sets up the ruler marks along the slider.
e.g.

KSlider slider( KSlider::Horizontal, parent );

slider.setRange( 0, 20 );
slider.setSteps( 5, 10 );

will create a horizontal slider with a range of 0 to 20 and small ruler
marks spaced 5 apart and long ruler marks placed 10 apart.

TODO

Currently clicking any where in the control moves the slider to that position.
This should be changed so that the slider may be grabbed and moved, or an
area on the left or right of the control clicked to move the slider in
lineStep() increments.
*/
class KSlider : public QSlider
{
  Q_OBJECT
    public:

  KSlider( QWidget *parent = 0L, const char *name = 0L );
  KSlider( Orientation o, QWidget *parent = 0L, const char *name = 0L );
  KSlider( int _minValue, int _maxValue, int _Step, int _value,
	   Orientation o, QWidget *parent = 0L, const char *name = 0L );

  virtual QSize sizeHint() const;

signals:
  void valueChanged( int value );
  void sliderPressed();
  void sliderMoved( int value );
  void sliderReleased();

protected:
  virtual void valueChange();
  virtual void rangeChange();
  virtual void paletteChange(const QPalette &);
  virtual void paintSlider(QPainter *painter, const QRect &rect );
  virtual void backgroundColorChange(const QPalette &);
  virtual void focusInEvent( QFocusEvent * );
  virtual void focusOutEvent( QFocusEvent * );

private:
  QPoint calcArrowPos( int val );
  void moveArrow( const QPoint &pos );
  void drawArrow( QPainter *painter, bool show, const QPoint &pos );
  void drawShadeLine( QPainter *painter );
  void drawTickMarks( QPainter *painter );
  void drawTicks( QPainter * p, int d, int w, int i=1 );
  void drawWinGroove(class QPainter *, short int);
  void drawFocusBar(QPainter *painter, const QRect & );
  virtual void paintEvent( QPaintEvent * );

  int checkWidth();
  bool isFocussed;
};

//-----------------------------------------------------------------------------

#endif
