/**********************************************************************

	--- Dlgedit generated file ---

	File: PrinterData.h
	Last generated: Tue Jun 24 22:14:32 1997

	DO NOT EDIT!!!  This file will be automatically
	regenerated by dlgedit.  All changes will be lost.

 *********************************************************************/

#ifndef PrinterData_included
#define PrinterData_included

#include <qdialog.h>
#include <qcombo.h>
#include <qradiobt.h>

#include "Data.h"

class PrinterData : public QDialog
{
    Q_OBJECT

public:

    PrinterData
    (
        QWidget* parent = NULL,
        const char* name = NULL
    );

    virtual ~PrinterData();

     Data *data;

public slots:


protected slots:

    virtual void Dpi300ButtonClicked();
    virtual void Dpi600ButtonClicked();
    virtual void EconomyButtonClicked();
    virtual void ResolutionComboBoxClicked(const char*);
    virtual void DensityComboClicked(const char*);
    virtual void PrinterComboClicked(const char*);
    virtual void PresentationButtonClicked();

protected:
    
    QButtonGroup* DpiButtonGroup;
    QButtonGroup* EconomyButtonGroup;
    QRadioButton* EconomyButton;
    QRadioButton* PresentationButton;
    QRadioButton* Dpi300Button;
    QRadioButton* Dpi600Button;
    QComboBox* DensityComboBox;
    QComboBox* ResolutionComboBox;
    QComboBox* PrinterComboBox;

};

#endif // PrinterData_included
