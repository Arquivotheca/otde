/**********************************************************************

	--- Qt Architect generated file ---

	File: KSircColourData.h
	Last generated: Sat Dec 13 00:46:18 1997

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef KSircColourData_included
#define KSircColourData_included

#include <qframe.h>
#include "kcolorbtn.h"

class KSircColourData : public QFrame
{
    Q_OBJECT

public:

    KSircColourData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~KSircColourData();

public slots:


protected slots:

    virtual void ok();
    virtual void cancel();

protected:
    KColorButton* ColourText;
    KColorButton* ColourInfo;
    KColorButton* ColourChan;
    KColorButton* ColourError;

};

#endif // KSircColourData_included
