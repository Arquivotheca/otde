/**********************************************************************

	--- Qt Architect generated file ---

	File: servercontroller.h
	Last generated: Sat Nov 29 08:50:19 1997
	
 Now Under CVS control.

 $$Id$$

 *********************************************************************/

#ifndef servercontroller_included
#define servercontroller_included

#include <qdict.h>

#include "servercontrollerData.h"

#include "ksircprocess.h"

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

public slots:
    // All slots are described in servercontroll.cpp file
    virtual void new_connection(); 
    virtual void new_ksircprocess(QString);
    virtual void add_toplevel(QString, QString); 
    virtual void delete_toplevel(QString, QString); 
    virtual void new_channel();
    virtual void new_toplevel(QString str);
    virtual void recvChangeChannel(QString, QString, QString);
    virtual void reuse();
    virtual void colour_prefs();

private:
    // Hold a list of all KSircProcess's for access latter.  Index by server 
    // name
  QDict<KSircProcess> proc_list;
  QPopupMenu *options, *connections;
  int reuse_id, join_id, server_id;

  int open_toplevels;

};
#endif // servercontroller_included
