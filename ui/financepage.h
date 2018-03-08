#ifndef FINANCEPAGE_H
#define FINANCEPAGE_H

#include <QObject>
#include <QDialog>
#include "mymessage.h"
#include "lockdrv.h"
#include "mylineedit.h"

class QStackedLayout;

class financepage : public QDialog
{
    Q_OBJECT
public:
    explicit financepage(QWidget *parent = 0);

    void readInfo(QString info);
    
signals:
    void resetTime();
public slots:

    void openBankCell();
    void skipGetCode();
    void quitPage();
    void getCode();
    void quitGetCode();

private:

    void initPage();
    void initGetCodePage();

    void setBtnEnable(bool status);

    void startOpencell();
    bool judgeCell();

    QStackedLayout *stack;

    QPushButton *openBtn;
    QPushButton *getCodeBtn;
    QPushButton *exitBtn;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

    CKeyBoardLineEdit *codeLineEdit;
    CKeyBoardLineEdit *phoneLineEdit;

    QString m1Card;

    CLockDrv lockoperation;

    CMessageBox myMessageBox;

    bool isWorking;
    
};

#endif // FINANCEPAGE_H
