/**********************************************************************

	--- Qt Architect generated file ---

	File: PrintDialogData.h
	Last generated: Mon Aug 10 09:14:26 1998

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef PrintDialogData_included
#define PrintDialogData_included

#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qframe.h>

class PrintDialogData : public QDialog
{
    Q_OBJECT

public:

    PrintDialogData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~PrintDialogData();

public slots:


protected slots:


protected:
    QFrame* frameBase;
    QPushButton* buttonOK;
    QPushButton* buttonCancel;
    QFrame* framePage;
    QLabel* labelUpperMargin;
    QLabel* labelHeader;
    QLabel* labelLowerMargin;
    QLabel* labelLeftMargin;
    QLabel* labelRightMargin;
    QLineEdit* leUpperMargin;
    QLineEdit* leLowerMargin;
    QLineEdit* leLeftMargin;
    QLineEdit* leRightMargin;

};

#endif // PrintDialogData_included
