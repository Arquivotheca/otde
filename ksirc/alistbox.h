#ifndef ALISTBOX_H
#define ALISTBOX_H

class aListBox;
class nickListItem;

#include <qobject.h>
#include <qwidget.h>
#include <qlistbox.h>
#include <qevent.h>

#include "irclistitem.h"

class nickListItem : public QListBoxItem
{
 public:
  nickListItem();
  ~nickListItem();

  int height ( const QListBox * ) const;
  int width ( const QListBox * ) const;
  const char* text () const;
  const QPixmap* pixmap () const;
  
  bool op();
  bool voice();

  void setOp(bool _op = FALSE);
  void setVoice(bool _voice = FALSE);

  void setText(const char *str);

  nickListItem &operator= ( const nickListItem & nli );
  
 protected:
  virtual void paint ( QPainter * );

 private:
  bool is_op;
  bool is_voice;

  QString string;
};

class aListBox : public QListBox
{
  Q_OBJECT;

public:
  aListBox(QWidget *parent = 0, const char *name = 0) : QListBox(parent,name)
    {
      clear();
      p_scroll = palette().copy();
    }

  void clear();

  void inSort ( nickListItem *);
  void inSort ( const char * text, bool top=FALSE);

  nickListItem *item(int index);

  bool isTop(int index);

  virtual void setPalette ( const QPalette & );

  int findNick(QString str);

signals:
   void rightButtonPress(int index);

protected:
  virtual void mousePressEvent ( QMouseEvent * );
  virtual int findSep();
  virtual int searchFor(QString nick, bool &found, bool top);

private:
  QPalette p_scroll;


};

#endif
