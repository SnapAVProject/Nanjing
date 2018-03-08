#ifndef CARDPICKPAGE_H
#define CARDPICKPAGE_H

#include <QDialog>
#include <QTableView>
#include "takeviewmodel.h"
#include "lockdrv.h"
#include "mymessage.h"
#include "cadvertisement.h"
#include "cardtakeinfo.h"

class cardpickpage : public QDialog
{
    Q_OBJECT
public:
    explicit cardpickpage(QWidget *parent = 0);

    void readInfo(QByteArray);


    
signals:
    void resetTime();
public slots:
    void openBox();
    void quitPage();

private:

    void initPage();
    bool againCheckCellProc(int &boxStatus,double &paidCost,QString deliveryId,QString openCell);

    void updateModelData(int row,int column,QVariant value);

    int startOpenLock(bool way);

    void reportInfo();

    CLockDrv lockoperation ;

    QLabel *payInfo;
    QPushButton *openBtn;
    QPushButton *quitBtn;
    QTableView *view;
    takeViewModel *model;
    QList <QStringList> modelData;
    double debtCost;    //自费的总费用
    double vipDebtCost; //年卡的总费用
    double mBalance;    //卡里面余额
    double vipRate;     //vip打折率

    double payCoinCard; //刷卡付费总计

    QMap<int,QStringList> openCellList;
    QMap<int,QStringList> closeCellList;

    QString mCardDate;  //卡是否过期

    QString m1Card;    //m1卡号
    QString terminal;  //终端编号

    QString image;   //图片

    CMessageBox myMessageBox;

    CAdvertisement advertisementPage;
    cardtakeinfo takeInfoPage;


};

#endif // CARDPICKPAGE_H
