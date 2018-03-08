#include "pricecheck.h"
#include "ui_pricecheck.h"
#include "mainctrl.h"
#include "configure/db.h"

#include <QDebug>

pricecheck::pricecheck(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pricecheck)
{
    ui->setupUi(this);

    initPage();
    connect(ui->signOut,SIGNAL(clicked()),this,SLOT(signOutPushButton_clicked()));
}

pricecheck::~pricecheck()
{
    delete ui;
}

void pricecheck::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setObjectName("QWidget");
    this->setStyleSheet("#QWidget{background-image: url(:/page/images/tariffs/beijing.png)}");

    ui->chargesType->setAlignment(Qt::AlignCenter);
    ui->chargesType->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:20px;color:rgb(0,0,0)");

    ui->courierframe->setObjectName("courier");
    ui->courierframe->setStyleSheet("QWidget#courier{border-image: url(:/page/images/tariffs/courier.png)}");
    bigPrice = new QLabel(ui->courierframe);
    bigPrice->resize(40,25);
    bigPrice->move(142,25);
    bigPrice->setAlignment(Qt::AlignCenter);
    bigPrice->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:15px;color:rgb(255,255,255);");

    midPrice = new QLabel(ui->courierframe);
    midPrice->resize(40,25);
    midPrice->move(142,50);
    midPrice->setAlignment(Qt::AlignCenter);
    midPrice->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:15px;color:rgb(255,255,255)");

    smallPrice = new QLabel(ui->courierframe);
    smallPrice->resize(40,25);
    smallPrice->move(142,75);
    smallPrice->setAlignment(Qt::AlignCenter);
    smallPrice->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:15px;color:rgb(255,255,255)");

    ui->userframe->setObjectName("user");
    ui->userframe->setStyleSheet("QWidget#user{border-image: url(:/page/images/tariffs/user.png)}");

    VIPEveryPrice = new QLabel(ui->userframe);
    this->VIPEveryPrice->resize(30,25);
    this->VIPEveryPrice->move(70,77);
    this->VIPEveryPrice->setAlignment(Qt::AlignCenter);
    this->VIPEveryPrice->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:15px;color:rgb(255,255,255)");

    VIPPrice = new QLabel(ui->userframe);
    this->VIPPrice->resize(30,25);
    this->VIPPrice->move(100,106);
    this->VIPPrice->setAlignment(Qt::AlignCenter);
    this->VIPPrice->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:15px;color:rgb(255,255,255)");

    APPVIPPrice = new QLabel(ui->userframe);
    this->APPVIPPrice->resize(30,25);
    this->APPVIPPrice->move(103,135);
    this->APPVIPPrice->setAlignment(Qt::AlignCenter);
    this->APPVIPPrice->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:15px;color:rgb(255,255,255)");

    ui->defaultdebframe->setObjectName("toll");
    ui->defaultdebframe->setStyleSheet("QWidget#toll{border-image: url(:/page/images/tariffs/toll.png)}");

    delayTime = new QLabel(ui->defaultdebframe);
    delayTime->resize(30,25);
    delayTime->move(10,55);
    delayTime->setAlignment(Qt::AlignCenter);
    delayTime->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:15px;color:rgb(255,255,255)");

    delayCoin = new QLabel(ui->defaultdebframe);
    delayCoin->resize(30,25);
    delayCoin->move(50,80);
    delayCoin->setAlignment(Qt::AlignCenter);
    delayCoin->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:15px;color:rgb(255,255,255)");

    outTime = new QLabel(ui->defaultdebframe);
    outTime->resize(30,25);
    outTime->move(0,108);
    outTime->setAlignment(Qt::AlignLeft);
    outTime->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:15px;color:rgb(255,255,255)");

    outCoin = new QLabel(ui->defaultdebframe);
    outCoin->resize(30,25);
    outCoin->move(80,105);
    outCoin->setAlignment(Qt::AlignCenter);
    outCoin->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:15px;color:rgb(255,255,255)");

    ui->signOut->resize(182,70);
    ui->signOut->setFlat(true);
    ui->signOut->setFocusPolicy(Qt::NoFocus);
    ui->signOut->setStyleSheet("background-color:transparent;background-image: url(:/public/images/public/exitBtn.png)");
}

void pricecheck::readInfo()
{
    QString bigCellTariffStr;
    QString centreCellTariffStr;
    QString smallCellTariffStr;
    QString vegetablesTariffStr;
    QString vipPercent;
    QString customerPrice;
    QString appVipPercent;
    QString vip;
    QString app;
    QStringList vipPercentTemp;
    QStringList appPercentTemp;



    ui->signOut->setEnabled(false);
    getTariffInformationResp respone;

    SCServerComm::instance()->getTariffInformation(&respone);

    ui->signOut->setEnabled(true);

    APPVIPPrice->show();
    ui->userframe->setStyleSheet("QWidget#user{border-image: url(:/page/images/tariffs/user.png)}");

    if(respone.success() != RET_TRUE)
    {
        return;
    }
    int priceType = respone.tariffTypeStr().toInt();
    ui->userframe->show();
    ui->courierframe->show();
    ui->defaultdebframe->show();


    switch(priceType)
    {
    case 1:
        ui->chargesType->setText(tr("免费服务"));
        ui->userframe->hide();
        ui->courierframe->hide();
        break;
    case 2:
        ui->chargesType->setText(tr("向快递员收费"));
        ui->userframe->hide();

        bigCellTariffStr = respone.bigCellTariffStr();
        centreCellTariffStr = respone.centreCellTariffStr();
        smallCellTariffStr = respone.smallCellTariffStr();
        vegetablesTariffStr = respone.vegetablesTariffStr();


        bigPrice->setText(bigCellTariffStr);
        midPrice->setText(centreCellTariffStr);
        smallPrice->setText(smallCellTariffStr);
  //      ui->vegetablePrice->setText(vegetablesTariffStr);
        break;

    case 3:
        ui->chargesType->setText(tr("向用户收费"));
        ui->courierframe->hide();

        vipPercent = respone.vipPercent();
        customerPrice = respone.customerPrice();
        appVipPercent = respone.weightPercent();

        vipPercentTemp = vipPercent.split(".");
        appPercentTemp = appVipPercent.split(".");

        VIPEveryPrice->setText(customerPrice);
        if("1" == vipPercentTemp.at(0))
        {
            VIPPrice->setText("不打");
        }
        else if(vipPercentTemp.size() > 1)
        {
            vip = vipPercentTemp.at(1);
            if(vipPercentTemp.at(1).size() > 1)
            {
                vip.insert(1,QString("."));
            }
            VIPPrice->setText(vip);
        }
        else
        {
            VIPPrice->setText(vipPercentTemp.at(0));
        }


        if("0" == respone.appPercent())
        {
            APPVIPPrice->hide();
            ui->userframe->setStyleSheet("QWidget#user{border-image: url(:/page/images/tariffs/user2.png)}");
        }
        else if("1" == appPercentTemp.at(0))
        {
            APPVIPPrice->setText("不打");
        }
        else if(appPercentTemp.size() > 1)
        {
            app = appPercentTemp.at(1);
            if(appPercentTemp.at(1).size() > 1)
            {
                app.insert(1,QString("."));
            }
            APPVIPPrice->setText(app);
        }
        else
        {
            APPVIPPrice->setText(appPercentTemp.at(0));
        }
        break;
    case 4:
        ui->chargesType->setText(tr("双向收费"));

        bigCellTariffStr = respone.bigCellTariffStr();
        centreCellTariffStr = respone.centreCellTariffStr();
        smallCellTariffStr = respone.smallCellTariffStr();
        vegetablesTariffStr = respone.vegetablesTariffStr();

        bigPrice->setText(bigCellTariffStr);
        midPrice->setText(centreCellTariffStr);
        smallPrice->setText(smallCellTariffStr);
        //ui->vegetablePrice->setText(vegetablesTariffStr);

        vipPercent = respone.vipPercent();
        customerPrice = respone.customerPrice();
        appVipPercent = respone.weightPercent();

        vipPercentTemp = vipPercent.split(".");
        appPercentTemp = appVipPercent.split(".");

        if("1" == vipPercentTemp.at(0))
        {
            VIPPrice->setText("不打");
        }
        else if(vipPercentTemp.size() > 1)
        {
            vip = vipPercentTemp.at(1);
            if(vipPercentTemp.at(1).size() > 1)
            {
                vip.insert(1,QString("."));
            }
            VIPPrice->setText(vip);
        }
        else
        {
            VIPPrice->setText(vipPercentTemp.at(0));
        }

        if("0" == respone.appPercent())
        {
            APPVIPPrice->hide();
            ui->userframe->setStyleSheet("QWidget#user{border-image: url(:/page/images/tariffs/user2.png)}");
        }
        else if("1" == appPercentTemp.at(0))
        {
            APPVIPPrice->setText("不打");
        }
        else if(appPercentTemp.size() > 1)
        {
            app = appPercentTemp.at(1);
            if(appPercentTemp.at(1).size() > 1)
            {
                app.insert(1,QString("."));
            }
            APPVIPPrice->setText(app);
        }
        else
        {
            APPVIPPrice->setText(appPercentTemp.at(0));
        }

        VIPEveryPrice->setText(customerPrice);
        break;

    default:
        ui->chargesType->setText(tr("免费服务"));
        ui->userframe->hide();
        ui->courierframe->hide();
        break;
    }
    QString defaultDebSwitch = respone.defaultDebSwitch();
    if(defaultDebSwitch == "no")
    {
        ui->defaultdebframe->hide();
    }
    QString defaultDebtdays = respone.defaultDebtdays();
    QString defaultDebtmoney = respone.defaultDebtmoney();

    delayTime->setText(defaultDebtdays);
    delayCoin->setText(defaultDebtmoney);
    outTime->setText(respone.expireStep());
    outCoin->setText(respone.stepMoney());


}

void pricecheck::signOutPushButton_clicked()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
