#ifndef KSIRCTOPLEVEL_H
#define KSIRCTOPLEVEL_H

class KSircTopLevel;
class kstInside;

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>  

#include <qwidget.h>
#include <qsocknot.h>
#include <qlined.h>
#include <qmlined.h>
#include <qlayout.h>
#include <qframe.h>
#include <qlistbox.h>
#include <qscrbar.h>
#include <qregexp.h>
#include <qaccel.h>
#include <qlcdnum.h>

#include <ktopwidget.h>
#include <kmenubar.h>
#include <kfm.h>
#include <ksimpleconfig.h>
#include <knewpanner.h>

//#include "ahtmlview.h"
#include "irclistitem.h"
#include "ahistlineedit.h"
#include "alistbox.h"
#include "messageReceiver.h"
#include "ksircprocess.h"
#include "KSTicker/ksticker.h"
#include "usercontrolmenu.h"
#include "irclistbox.h"

class kstInside : QFrame 
{
  Q_OBJECT
  friend class KSircTopLevel;
 public:
  kstInside ( QWidget * parent=0, const char * name=0, WFlags
	     f=0, bool allowLines=TRUE );
  ~kstInside();


 protected:
  virtual void resizeEvent ( QResizeEvent * );
 private:
  KNewPanner *pan;
  KSircListBox *mainw;
  aHistLineEdit *linee;
  aListBox *nicks;

};

class KSircTopLevel : public KTopLevelWidget,
		      public KSircMessageReceiver
{
  Q_OBJECT;
public:
  /**
    * Constructor, needs the controlling ksircprocess passed to it, so
    * we can make the ksircmessage receiver happy.
    */
  KSircTopLevel(KSircProcess *_proc, char *cname=0L, const char *
		name=0);
  /**
    * Destructor, destroys itself.
    */
  ~KSircTopLevel();

  /**
    * Reimplement show() to popup the menu bars and toolbar items
    */
  virtual void show();

signals:
  /**
    * signals thats the toplevel widget wishes to
    * output a new line.  The line is correctly
    * linefeed terminated, etc.
    */
  void outputLine(QString&);
  /**
    * open a new toplevel widget with for the
    * channel/user QString.
    */
  void open_toplevel(QString);
  /**
    * emittedon shutdown, indicating that
    * this window is closing. Refrence to outselves
    * is include.
    */
  void closing(KSircTopLevel *, char *);
  /**
    * emitted when we change channel name
    * on the fly.  old is the old channel name, new
    * is the new one.
    */
  void changeChannel(QString, QString);
  /**
    * emitted to say that KSircTopLevel
    * (this) is now the window with focus.  Used by
    * servercontroller to set the !default window.
    */
  void currentWindow(KSircTopLevel *);
  /**
    * Window has changed size.  Used by ircListItem's to
    * resize and readjust their paint'ing prefrences.
    */
  void changeSize();
  /**
   * Stop updating list item sizes, majour changes comming through
   *
   */
  void freezeUpdates(bool);
  /** 
   * Emitted when we get object destroyed
   */
  void objDestroyed(KSircTopLevel *);

public slots:
  /**
    * Line recieved that should be printed on the screen. Unparsed, and
    * ready processing.  This is a single line, and it NOT \n
    * terminated.
  */
  void sirc_receive(QString str);
  /**
    * When enter is pressed, we read the SLE and output the results
    * after processing via emitting outputLine.
    */
  void sirc_line_return(); 

  /** 
    * Reimplement the ksircmessagereceiver control messages.  These
    * are parsed and dealt with quickly.
    */
  void control_message(int, QString); 

protected slots:
    /**
      * When the rightMouse button is pressed in the nick list, popup
      * the User popup menu at his cursor location.
      */
   void UserSelected(int index); 
   /**
     * Menu items was selected, now read the UserControlMenu and
     * reupdate the required menus.
     */
   void UserParseMenu(int id);
   /**
     * Page down accel key.  Tells the mainw to scroll down 1 page.
     */
   void AccelScrollDownPage();
   /**
     * Page Up accell key.  Tells the mainw to scroll down 1 page.
     */
   void AccelScrollUpPage();
   /**
     * Tied to the prior nick accel. When pressed, re-writes the SLE
     * with the prior nick to /msg us.
     */
   void AccelPriorMsgNick();
   /**
     * Tied to next nick accel.  When presed, re-writes the SLE when
     * the next nick to mesage us in the list. 
     */
   void AccelNextMsgNick();
   /**
     * Slot to termiate (close) the window.
     */
   void terminate() { close(1); }
   /** 
     * Called when the user menu is finished and the popup menu needs
     * to be updated.
     */
   void UserUpdateMenu();
   /**
     * Open the new channel/window selector.
     */
   void newWindow();
   /** 
     * We've received focus, let's make us the default, and issue a
     * /join such that we default to the right channel.
     */
   void gotFocus();
   /**
     * We've lost focus, set ourselves as loosing focus.
     */
   void lostFocus();
   /**
     * Create and popup the ticker.  Make sure to restore it to the
     * last position is was at.
     */
   void showTicker();
   /**
     * Delete the ticker and ppoup the main window
     */
   void unHide();
   /**
     * Since we can't cut from the listbox for cut requests we popup a
     * cut box and edit this instead.
     */
   void openCutWindow();
   /**
     * On a middle mouse button press we call pasteToWindow which
     * reads the clip board and pastes into the main listbox.
     */
   void pasteToWindow();
   /**
     * On a TAB key press we call TabNickCompletion which
     * reads the last thing in linee matches it with a nick and 
     * puts it back into the line.
     */
   void TabNickCompletion();
   /**
     * Signals a Line Change in linee
     */
   void lineeTextChanged(const char *);
   /**
     *  Move the display to or from the root window
     *
     */
   void toggleRootWindow();
   /**
    * Slot connected to destroy signal
    */
   void iamDestroyed();

protected:
   /**
     * Redfine closeEvent to emit closing and delete itself.
     */
   virtual void closeEvent(QCloseEvent *);
   /** 
     * catch and handle all resizeEvents so we can notify
     * ircListItem's correctly
     */
   virtual void resizeEvent(QResizeEvent *);
   /**
     * Searches through the nick list and finds the nick with the best match.
     * which, arg2, selects nicks other than the first shouldarg1 match more
     * than 1 nick.
     */
   virtual QString findNick(QString, uint which = 0);

private:
  bool continued_line;
  kstInside *f;
  KNewPanner *pan;
  QMenuBar *kmenu;
  KToolBar *ktool;
  QLCDNumber *lagmeter;
  enum {PING = 10, TOPIC = 20};
  KSircListBox *mainw;
  aHistLineEdit *linee;
  aListBox *nicks;
  //  QVBoxLayout *gm;
  //  QHBoxLayout *gm2;

  KSircProcess *proc;

  QList<QString> contents;
  int lines;

  bool Buffer;
  QStrList *LineBuffer;

  ircListItem *parse_input(QString &string);
  void sirc_write(QString &str);

  QPopupMenu *user_controls;
  static QList<UserControlMenu> *user_menu;
  int opami;

  QAccel *accel;
  QStrIList nick_ring;

  static QPixmap *pix_info;
  static QPixmap *pix_star;
  static QPixmap *pix_bball;
  static QPixmap *pix_greenp;
  static QPixmap *pix_bluep;
  static QPixmap *pix_madsmile;

  /**
    * The channel name that we belong too.
    */
  char *channel_name;

  /**
    * Caption at the top of the window.
    */
  QString caption;

  /**
    * Does the window have focus? 1 when yes, 0 when no.
    */
  int have_focus;

  /** 
    * Number of time tab has been pressed.  Each time it's pressed
    * roll through the list of matching nicks.  
    */
  int tab_pressed;
  /**
    * When tabs pressed save the line for use at a latter date.
    */
  QString tab_saved;
    

  // Ticker specific variables.

  /**
    * Main pointer to ksticker.  Caution, it doesn't always exist!
    * Set to 0 ifwhen you delete it.
    */
  KSTicker *ticker;
  /**
    * Size and position of the main window. Main window is returns this
    * this position when it reappears.
    */
  QRect myrect;
  /**
    * Position of the main window
    */
  QPoint mypoint;
  /**
    * Ticker's size and geometry
    */
  QRect tickerrect;
  /**
    * Tickers position
    */
  QPoint tickerpoint;

  /**
    * Never open mmore then one dialog box
    */
  bool prompt_active;

  /**
    * True when we are on the root window, false otherwise
    *
    */
  bool on_root;

};

#endif
