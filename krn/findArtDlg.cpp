#include "findArtDlg.h"

#include <qcombo.h>
#include <qchkbox.h>
#include <qlabel.h>
#include <qlined.h>
#include <qpushbt.h>
#include <kapp.h>

#include "typelayout.h"
#include "tlform.h"


#define Inherited QWidget

#include "findArtDlg.moc"

findArtDlg::findArtDlg (QWidget* parent,const char* name):Inherited( parent, name )
{

    TLForm *f=new TLForm("expiration",
                         klocale->translate("KRN - Article Searcher"),
                         this);

    KTypeLayout *l=f->layout;

    l->addGroup("entries","",true);

    l->addLabel("l1",klocale->translate("Find Articles With:"));
    l->newLine();
    expr=(QLineEdit *)(l->addLineEdit("expr","")->widget);
    l->newLine();

    l->addLabel("l2",klocale->translate("In Field:"));
    l->newLine();

    QStrList *fields=new QStrList;
    fields->append( "Subject" );
    fields->append( "Sender" );
    fields->append( "Cached Body" );
    fields->append( "Body" );
    field=(QComboBox *)(l->addComboBox("field",fields)->widget);
    delete fields;

    l->newLine();
    
    l->addGroup("gg","",false);
    casesen=(QCheckBox *)(l->addCheckBox("casesen",
                                         klocale->translate("Case Sensitive"),
                                         false)->widget);
    wildmode=(QCheckBox *)(l->addCheckBox("wildmode",
                                         klocale->translate("Wildcard Mode"),
                                         false)->widget);
    l->endGroup();
    
    l->endGroup();

    l->newLine();

    l->addGroup("buttons","",false);
    QPushButton *b1=(QPushButton *)(l->addButton("b1",klocale->translate("Find"))->widget);
    QPushButton *b2=(QPushButton *)(l->addButton("b2",klocale->translate("Done"))->widget);
    l->endGroup();

    l->activate();
    
    expr->setFocus();
    
    QObject::connect(b1,SIGNAL(clicked()),this,SLOT(find()));
    QObject::connect(b2,SIGNAL(clicked()),this,SLOT(close()));
    QObject::connect(expr,SIGNAL(returnPressed()),this,SLOT(find()));
}


findArtDlg::~findArtDlg()
{
}

void findArtDlg::close()
{
    hide();
}

void findArtDlg::find()
{
    emit FindThis (expr->text(),field->currentText(),
                   casesen->isChecked(),wildmode->isChecked());
}
