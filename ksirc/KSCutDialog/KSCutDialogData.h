/**********************************************************************

	--- Qt Architect generated file ---

	File: KSCutDialogData.h
	Last generated: Tue Dec 30 00:04:04 1997

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef KSCutDialogData_included
#define KSCutDialogData_included

#include <qdialog.h>
#include <qmlined.h>

class KSCutDialogData : public QDialog
{
    Q_OBJECT

public:

    KSCutDialogData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~KSCutDialogData();

public slots:


protected slots:

    virtual void terminate();

protected:
    QMultiLineEdit* IrcMLE;

};

#endif // KSCutDialogData_included
