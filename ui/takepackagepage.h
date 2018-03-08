#ifndef TAKEPACKAGEPAGE_H
#define TAKEPACKAGEPAGE_H

#include <QDialog>
#include "mylineedit.h"
#include "debtpage.h"
#include "lockdrv.h"
#include "mymessage.h"
#include "servercomm.h"
#include "ctakeopencell.h"
#include "cadvertisement.h"
#include <QStackedLayout>
#include <QLabel>
#include <QTimer>
#include <QEventLoop>
#include <QProgressDialog>


namespace Ui {
    class takepackagepage;
}

class takepackagepage : public QDialog {
    Q_OBJECT
public:
    takepackagepage(QWidget *parent = 0);
    ~takepackagepage();

    void initPage();

    void payCoin();
    void payCard(QString);

signals:
    void resetTime();

public slots:
    void takePack_slots();
    void quitTakePack_slots();
    void againTakeCode_slots();
    void getCode_slots();
    void quitGetCode_slots();
    void startOpenCell();



protected:
    void changeEvent(QEvent *e);

private:
    Ui::takepackagepage *ui;


    QStackedLayout *stack;
    QPushButton *okBtn_takePack;
    QPushButton *cancelBtn_takePack;
    QPushButton *againBtn;
    QPushButton *okBtn_getCode;
    QPushButton *cancelBtn_getCode;

    CDebtPage debtInfoPage;
    CTakeOpenCell takeCellInfoPage;
    CAdvertisement advertisementPage;

    void initGetCode();
    void initTakePack();
    void reportServer();
    void handleCardInfo(getVIPCardInfoResp*);

    void notOpenRecordCoin();

    void getExpressDebt();                //获取欠款相关信息
    void localTakePackage(QString randomCode);             //本地取包裹
    void openBoxLock(qint8,qint8);
    int againCheckCellProc();            //检测箱门是否打不开 1正常 0 已被取走 -1 取件打不开
    int againLocalCheckCellProc();       //检测本地箱门是否打不开 1正常 0 已被取走 -1 取件打不开

    void setTakePackEnable(bool);         //设置取件按钮可不可用

    CKeyBoardLineEdit *takePack_lineEdit;

    CKeyBoardLineEdit *getCode_lineEdit;

//    bool payCoinStatus;                    //投币状态
    double mDebtCost;                      //滞留费
    double mOwnerCost;                     //业主付费
    double mHashPay;                       //用户已付费用
    double mDebtMoney;                     //总欠费多少（包括滞留费、业主需付的费用）
    double mTempDebt;                      //总欠费多少（中间变量）
    double mVipPercent;                    //刷卡打折率
    qint8  mCellType;                      //箱门类型

    double mPayMoney;                      //刷卡应付多少
    double mBalance;                       //卡余额多少
    double mCoin;                          //投币的钱

    QString openCell;                      //要打开的箱子号
    QString deliveryId;                    //派件单号
    QString mM1Card;                        //用户刷卡支付
    QString mMobilePhone;                  //用户手机号码

    QString forMySelf;                     //刷卡支付为自己还是别人 yes 自己 no 别人
    QString loseDate;                      //是否卡过期

    bool mCoinWay;                         //投币支付方式
    bool mCardWay;                         //刷卡支付方式
    bool mFakeCoin;                        //没有按流程投币(箱门打不开时不计数）
    bool localWay;                         //断网取件方式

    QStringList waitCheckBoxClose;         //检测箱子是否关闭
    QStringList notOpenCellDeliveryId;     //打不开的箱门存的派件ID

    CMessageBox myMessageBox;

    CLockDrv lockoperation;

    QProgressDialog progress;

    bool isWorking;




 //   QByteArray byteArry;


/*
    void delayTime(long time)
    {
        QTimer timer;
        QEventLoop loop;
        timer.setInterval(time);
        timer.setSingleShot(true);
        timer.start();

        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        loop.exec();
    }
  */
};

#endif // TAKEPACKAGEPAGE_H
