/**********************************************************************

	--- Qt Architect generated file ---

	File: servercontroller.h
	Last generated: Sat Nov 29 08:50:19 1997
	
 Now Under CVS control.

 $$Id$$

 *********************************************************************/

#ifndef servercontroller_included
#define servercontroller_included

class servercontroller;
class ServMessage;

#include <qdict.h>
#include <qpixmap.h> 

#include "servercontrollerData.h"

#include "ksircprocess.h"

class ProcCommand // ServerController message
{
 public:
  static enum {
    addTopLevel,
    deleteTopLevel,
    procClose,
    newChannel,
    changeChannel,
    nickOnline,
    nickOffline
  } command;
};


class ServCommand // ServerController message
{
 public:
  static enum {
    updateFilters
  } command;
};

class servercontroller : public servercontrollerData
{
    Q_OBJECT

public:

    servercontroller
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~servercontroller();

signals:
    /**
      * Filter rules have changed, need to re-read and update.
      */
    virtual void filters_update();

    void ServMessage(QString server, int command, QString args);

public slots:
    // All slots are described in servercontroll.cpp file
    /**
      * Creates popup asking for new connection
      */
    virtual void new_connection(); 
    /**
      *  Args:
      *    QString: new server name or IP to connect to.
      *  Action:
      *	 Creates a new sirc process and window !default connected to the 
      *	 server.  Does nothing if a server connection already exists.
      */
    virtual void new_ksircprocess(QString);
    /**
      * Creates popup asking for new channel name
      */
    virtual void new_channel();
    /**
      *  Args:
      *    str: name of the new channel to be created
      *  Action:
      *    Sends a signal to the currently selected server in the tree
      *    list and join the requested channel.  Does nothing if nothing
      *    is selected in the tree list.
      */
    virtual void new_toplevel(QString str);
    /**
      *  Action:
      *     Toggles the Global option to reuse the !default window on each
      *     call.  Sets the menu item.   
      */
    virtual void reuse();
    /**
      * Action:
      *     Notify all ksircprocess' to update filters
      */
    virtual void slot_filters_update();
    virtual void autocreate();
    virtual void colour_prefs();
    virtual void font_prefs();
    virtual void font_update(const QFont&);
    virtual void filter_rule_editor();
    virtual void configChange();
    virtual void nickcompletion();
    virtual void help_general();
    virtual void help_colours();
    virtual void help_filters();
    virtual void help_keys();
    virtual void about_ksirc();

    virtual void ProcMessage(QString server, int command, QString args);

private:
    // Hold a list of all KSircProcess's for access latter.  Index by server 
    // name
  QDict<KSircProcess> proc_list;
  QPopupMenu *options, *connections;
  int reuse_id, join_id, server_id, auto_id, nickc_id;

  int open_toplevels;

  QPixmap *pic_channel;
  QPixmap *pic_server;
  QPixmap *pic_gf;
  QPixmap *pic_run;
  QPixmap *pic_ppl;

};
#endif // servercontroller_included
