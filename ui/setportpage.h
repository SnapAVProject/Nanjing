#ifndef SETPORTPAGE_H
#define SETPORTPAGE_H

#include <QDialog>
#include <QPushButton>
#include <QComboBox>
#include"mylineedit.h"
#include "mymessage.h"
#include "lockdrv.h"

class QStackedLayout;

namespace Ui {
class setportpage;
}

class setportpage : public QDialog
{
    Q_OBJECT
    
public:
    explicit setportpage(QWidget *parent = 0);
    ~setportpage();

    void init();
    bool setinfo(QString,QString,QString,QString,QString);
    void readinfo();

    void recordCoin();

    void upsTest();

private:

    void setNetworkTeamHide();
    void setNetworkTeamShow();

    bool loadcodeScan();

    void writeResolvFile();

signals:
    void resetTime();

public slots:
    void okbtn_slots();
    void openCell_slot();
    void isSendImage_slot();
    void isCheckGoods_slot();
    void cutPage_slot(int);

    void coinTest_slot();
    void setTime_slot();
    void catchPhoto_slot();
    void network_slot();

    void setIPAddr_slot();
    void checkGoods_slot();

    void choiceNetworkBoxChange(int);


protected:
    void closeEvent(QCloseEvent *);

    bool eventFilter(QObject *, QEvent *);
    
private:
    Ui::setportpage *ui;

    QLabel *catchPhotoLabel;
    QPushButton *openBoxBtn;
    QPushButton *sendImageBtn;
    QPushButton *checkGoodsBtn;
    QPushButton *startTestBtn;
    QPushButton *setTimeBtn;
    QPushButton *photoBtn;
    QPushButton *networkBtn;
    QPushButton *goodsBtn;

    bool sendStatus;

    QWidget *page;
    QStackedLayout *stack;

    CKeyBoardLineEdit *terminalId_lineEdit;
    CKeyBoardLineEdit *portId_lineEdit;
    CKeyBoardLineEdit *serveraddr_lineEdit;

    CKeyBoardLineEdit *openCell_lineEdit;
    CKeyBoardLineEdit *md5Code_lineEdit;
    CKeyBoardLineEdit *cityId_lineEdit;
    CKeyBoardLineEdit *goodsCheck_lineEdit;

    QLineEdit *coinTest_lineEdit;
    QLineEdit *scan_lineEdit;
    QLineEdit *network_lineEdit;
    QLineEdit *ups_lineEdit;

    QDateTimeEdit *dateTimeEdit;

    CMessageBox myMessageBox;

    CLockDrv lockoperation;


    CKeyBoardLineEdit *ipAddr_lineEdit;
    QPushButton *ipAddrBoxBtn;

    QComboBox *mChoiceNetworkBox;
    QLabel *mIpSetLable;
    CKeyBoardLineEdit *mIPSet_lineEdit;
    QLabel *mNetmaskLable;
    CKeyBoardLineEdit *mNetmaskSet_lineEdit;
    QLabel *mDefaultGwSetLable;
    CKeyBoardLineEdit *mDefaultGwSet_lineEdit;
    QLabel *mDnsLable1;
    CKeyBoardLineEdit *mDns1Set_lineEdit;
    QLabel *mDnsLable2;
    CKeyBoardLineEdit *mDns2Set_lineEdit;




};

#endif // SETPORTPAGE_H
