#ifndef MANAGEPAGE_H
#define MANAGEPAGE_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include "lockdrv.h"
#include "mylineedit.h"
#include "mymessage.h"


namespace Ui {
    class managepage;
}

class managepage : public QDialog {
    Q_OBJECT
public:
    managepage(QWidget *parent = 0);
    ~managepage();

    void init();
    void readinfo();

signals:
    void resetTime();

public slots:

    void isSendImage_slot();
    void openCell_slot();

    void setIPAddr_slot();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *);

private:
    Ui::managepage *ui;

    CKeyBoardLineEdit *openCell_lineEdit;
    CKeyBoardLineEdit *portId_lineEdit;
    CKeyBoardLineEdit *serveraddr_lineEdit;
    CKeyBoardLineEdit *md5Code_lineEdit;
    CKeyBoardLineEdit *ipAddr_lineEdit;

    QPushButton *sendImageBtn;
    QPushButton *openBoxBtn;
    QPushButton *ipAddrBoxBtn;

    bool sendStatus;

    CMessageBox myMessageBox;

    CLockDrv lockoperation;

};

#endif // MANAGEPAGE_H
