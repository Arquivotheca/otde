//Search dialog

#ifndef SEARCH_H
#define SEARCH_H

#include <qdialog.h>
#include <qlined.h>
#include <qchkbox.h>
#include <qradiobt.h>
#include <qlistbox.h>

//#include <keditcl.h>
#include "kwview.h"

class SearchDialog : public QDialog {
    Q_OBJECT
  public:
    SearchDialog(const char *searchFor, const char *replaceWith,
      int flags, QWidget *parent, const char *name = 0);
    const char *getSearchFor();
    const char *getReplaceWith();
    int getFlags();
//  signals:
//    void search();
//    void doneSearch();
  protected slots:
    void okSlot();

  protected:
    QLineEdit *search;
    QLineEdit *replace;

    QCheckBox *opt1;
    QCheckBox *opt2;
    QCheckBox *opt3;
    QCheckBox *opt4;
    QCheckBox *opt5;
    QCheckBox *opt6;
};

class ReplacePrompt : public QDialog {
    Q_OBJECT
  public:
    ReplacePrompt(QWidget *parent, const char *name = 0);
  protected slots:
    void no();
    void all();
    virtual void done(int);
};

class GotoLineDialog : public QDialog {
    Q_OBJECT
  public:
    GotoLineDialog(int line, QWidget *parent, const char *name = 0);
    int getLine();
  protected:
    QLineEdit *e1;
};

class SettingsDialog : public QDialog {
    Q_OBJECT
  public:
    SettingsDialog(int flags, int wrapAt, int tabWidth, QWidget *parent, const char *name = 0);
    int getFlags();
    int getWrapAt();
    int getTabWidth();
  protected:
    QCheckBox *opt1;
    QCheckBox *opt2;
    QCheckBox *opt3;
    QCheckBox *opt4;
    QCheckBox *opt5;
    QCheckBox *opt6;
    QCheckBox *opt7;
    QCheckBox *opt8;
    QCheckBox *opt9;
    QCheckBox *opt10;
    QCheckBox *opt11;
    QCheckBox *opt12;
    QLineEdit *e1;
    QLineEdit *e2;
};


#endif //SEARCH_H
