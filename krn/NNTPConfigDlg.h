#ifndef NNTPConfigDlg_included
#define NNTPConfigDlg_included

#include <qdialog.h>

class QPushButton;
class QLineEdit;
class QCheckBox;
class QLineEdit;

class NNTPConfigDlg : public QDialog
{
    Q_OBJECT

public:
    NNTPConfigDlg (QWidget* parent = NULL,const char* name = NULL);

    virtual ~NNTPConfigDlg();

    QPushButton* b1;
    QPushButton* b2;
    QLineEdit* servername;
    QCheckBox* connectatstart;
    QCheckBox* authenticate;
    QCheckBox* silentconnect;
    QLineEdit* username;
    QLineEdit* password;
    QLineEdit* smtpserver;
private slots:
        save();
};
#endif // NNTPConfigDlg_included
