/**********************************************************************

	--- Qt Architect generated file ---

	File: StringListSelectAndReorderSetData.h
	Last generated: Fri Aug 7 13:31:31 1998

	DO NOT EDIT!!!  This file will be automatically
	regenerated by qtarch.  All changes will be lost.

 *********************************************************************/

#ifndef StringListSelectAndReorderSetData_included
#define StringListSelectAndReorderSetData_included

#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlistbox.h>

class StringListSelectAndReorderSetData : public QFrame
{
    Q_OBJECT

public:

    StringListSelectAndReorderSetData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~StringListSelectAndReorderSetData();

public slots:


protected slots:

    virtual void unselectPressed();
    virtual void down();
    virtual void up();
    virtual void selectPressed();

protected:
    QListBox* lbPossible;
    QPushButton* buttonSelect;
    QPushButton* buttonUnselect;
    QListBox* lbSelected;
    QPushButton* buttonUp;
    QPushButton* buttonDown;
    QLabel* labelPossible;
    QLabel* labelSelected;

};

#endif // StringListSelectAndReorderSetData_included
