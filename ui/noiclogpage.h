#ifndef NOICLOGPAGE_H
#define NOICLOGPAGE_H

#include <QDialog>
#include "mylineedit.h"
#include "respone.h"
#include "mymessage.h"
#include <QStackedLayout>
#include <QBoxLayout>
#include <QProgressDialog>

class noiclogpage : public QDialog
{
    Q_OBJECT
public:
    noiclogpage();
    ~noiclogpage();

    void initPage(QByteArray data);

signals:
    void resetTime();

public slots:
    void getSaveCode_slot();
    void logSavePage_slot();
    void cancelSaveCode_slot();
    void cancelLogSavePage_slot();
    void register_slot();
    void takeDelayPackage();


private:
    QStackedLayout *stack;

    void initNoM1Log();
    void initM1Code();

    void startNoIcLog();
    void startOpenCellNoIcLog();
    void startDetainNoIcLog();
    void startCourierTakeNoIcLog();

    QByteArray waitconfirmByteArray(m1CardLoginResp &respone,QString personType,bool isFix);   //等待确认的包裹信息
    QByteArray savecellByteArray(QByteArray param);               //存件的箱体可用量信息 param 表示M1卡


    QPushButton *okBtn_getSaveCode;
    QPushButton *cancelBtn_getSaveCode;
    QPushButton *registerBtn;
    QPushButton *okBtn_logSave;
    QPushButton *cancelBtn_logSave;

    CKeyBoardLineEdit *phone_edit;
    CKeyBoardLineEdit *take_edit;

    CMessageBox myMessageBox;
    CMessageBox myBox;
    QProgressDialog progress;

    QString mM1Card;
    QString mMobilePhone;

    QString logStatus;

    bool isWorking;


};

#endif // NOICLOGPAGE_H
