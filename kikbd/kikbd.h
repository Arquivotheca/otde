#ifndef KKEYSWITCH_H
#define KKEYSWITCH_H

#include <qintdict.h>
#include <qlist.h>
#include <qwidget.h>
#include <qpushbt.h>
#include <qlabel.h>
#include <qpopmenu.h>
#include <kapp.h>
#include <X11/Xlib.h>
#include "keymap.h"
#include "kikbdconf.h"

class KiKbdButton: public QLabel {
  Q_OBJECT
 protected:
  void mousePressEvent(QMouseEvent*);
 public:
  KiKbdButton(QWidget*);
 signals:
  void showMenu();
  void clicked();
};

/**
   windows list. Window Id, keyboard map Id, window class Id
*/
class WindowEntry {
 public:
  Window  window;
  short   map, hotmap;
  short   classId;
  WindowEntry(Window w, int m=0, int h=1, int c=0)
    :window(w), map(m), hotmap(h), classId(c){}
};
class WindowList: public QListT<WindowEntry> {
 protected:
  virtual int compareItems(GCI, GCI);
 public:
  WindowList();
  ~WindowList(){}
  bool findWindow  (Window win) {
    WindowEntry entry(win);
    return find(&entry)!=-1;
  }
};
/**
   window class list. Class name, keyboard map Id.
*/
class WindowClassEntry {
 public:
  QString name;
  short   map, hotmap;
  WindowClassEntry(const char* s, short m=0, short h=1)
    :name(s), map(m), hotmap(h){}
};
class WindowClassList: public QListT<WindowClassEntry> {
 protected:
  virtual int compareItems(GCI, GCI);
 public:
  WindowClassList();
  ~WindowClassList(){}
};

/**
   main application class
*/
class KiKbdApplication: public KApplication {
  Q_OBJECT
 public:
  static void error(const char*, const char* s1=0, const char* s2=0);
  static void warning(const char*, const char* s1=0, const char* s2=0);
 protected:
  /**
     internal variables
  */
  QWidget              *topWidget;
  KiKbdButton          *button;
  QPopupMenu           *menu;
  XModifierKeymap      *modifs;
  WindowList            windowList;
  WindowClassList       classList;
  QList<KeyMap>         keyMaps;
  KeySyms               globalKeySyms;
  bool                  isFirstKey, isSecondKey;
  bool                  isToggleCaps, isToggleAlt;
  bool                  hotList;
  bool                  inConfig;
  int                   autoMenuRequestCount, altSwitchCount;
  int                   hotmap;
  /**
     configurations variables
  */
  QPalette normalPalette, capsPalette, altPalette, forPelette;
  bool     keyboardBeep, autoMenu, saveClasses, docked;
  int      input;
  KeySym   firstKey, secondKey, capsKey;
  KeySym   altKey;
  Atom     KiKbdReconfig, KiKbdRotate, KiKbdIdentity;
 protected:
  virtual bool x11EventFilter(XEvent *);
  /**
     select input from this windows
  */
  void selectWindowInput(Window);
  void selectRecursivelyInput(Window);
  /**
     forget this window
  */
  void forgetWindow(Window);
  /**
     find some properties of window
  */
  int isTopLevelWindow(Window);
  int isWmWindow(Window);
  QString windowClass(Window);
  bool    windowCheckProperty(Window, Atom);
  Window  findKiKbdWindow(Window win=0);
  void    sendCommand(Atom);
  bool    removeModifier(KeySym);
  bool    addModifier(KeySym,int);
 public:
  KiKbdApplication(int, char**);
  ~KiKbdApplication();
  /**
     get mouse pointer on the root window
  */
  QPoint getPointer();
  /**
     run this at exit
  */
  void atExit();
  void setKeyMapTo(unsigned, bool ch=TRUE);
 public slots:
  /**
     load configuration
  */
  void loadConfig();
  /**
     show popup menu
  */
  void showMenu();
  void activateMenu(int);
  void autoMenuRequest();
  void altSwitchTimer();
 /**
    changing map
 */
  void rotateKeyMap();
  void toggleCaps(bool);
  void toggleAlt(bool);
  void save();
  void setPalette();
};

#endif
