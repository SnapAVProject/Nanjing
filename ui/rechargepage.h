#ifndef RECHARGEPAGE_H
#define RECHARGEPAGE_H

#include <QDialog>
#include "mymessage.h"
#include "crechargeinfo.h"
#include "mylineedit.h"

class QPushButton;

namespace Ui {
class rechargepage;
}

class rechargepage : public QDialog
{
    Q_OBJECT
    
public:
    explicit rechargepage(QWidget *parent = 0);
    ~rechargepage();

    void initPageInfo();

signals:
    void resetTime();

public slots:
    void okbtn_slots();
    void cancelbtn_slots();

    void commitInfo_slots();

    void readCardInfo(QString);

protected:
    bool eventFilter(QObject *, QEvent *);
    
private:
    Ui::rechargepage *ui;

    void initPage();
    void setBtnEnable(bool);

    bool isWorking;

    QPushButton *okBtn;
    QPushButton *cancelBtn;
    CKeyBoardLineEdit *m1CardLineEdit;
    CKeyBoardLineEdit *pwdLineEdit;

    CMessageBox myMessageBox;
    CRechargeInfo rechargeInfo;

    QString topDate;

    qint8 topType;


};

#endif // RECHARGEPAGE_H
