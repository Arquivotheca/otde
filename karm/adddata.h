/**********************************************************************

	--- Dlgedit generated file ---

	File: adddata.h
	Last generated: Fri Jun 20 07:42:44 1997

	DO NOT EDIT!!!  This file will be automatically
	regenerated by dlgedit.  All changes will be lost.

 *********************************************************************/

#ifndef KAddDlgData_included
#define KAddDlgData_included

#include <qdialog.h>
#include <qlined.h>
#include <qpushbt.h>

class KAddDlgData : public QDialog
{
    Q_OBJECT

public:

    KAddDlgData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~KAddDlgData();

public slots:

    virtual void cancelClicked();
    virtual void okClicked();

protected slots:


protected:
    QLineEdit* _taskName;
    QLineEdit* _taskTime;
    QPushButton* _okButton;
    QPushButton* _cancelButton;

};

#endif // KAddDlgData_included