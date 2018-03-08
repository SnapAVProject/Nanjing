#include "homepage.h"
#include "ui_homepage.h"
#include "mainctrl.h"
#include "exceptioncomm.h"
#include "db.h"
#include <QDebug>
#include <QThread>
#include <QDateTime>
#include <QLabel>

HomePage::HomePage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);

    inithomepage();

    promptInfo = new CPromptInfo();

    timer = new QTimer(this);

    timer->start(1000);

    connect(promptInfo,SIGNAL(logRequest()),this,SLOT(noicLog_clicked()));

    connect(timer,SIGNAL(timeout()),this,SLOT(showCurTime()));
}

HomePage::~HomePage()
{
    delete ui;
}


void HomePage::getWeather()
{

    QTimer *weatherTimer = new QTimer(this);
    weatherTimer->setInterval(60*5000);
    weatherTimer->setSingleShot(true);
    weatherTimer->start();

    connect(weatherTimer,SIGNAL(timeout()),this,SLOT(showWeather()));
}

bool HomePage::showWeather()
{
    QString city = SCDatBase::instance()->getCityId();
    getWeacherInfoResp respone;
    SCExceptioncomm::instance()->getWeacherInfo(city,&respone);

    if(respone.success() == RET_TRUE)
    {
        ui->temperatureInfo->setText(respone.temperature());
        ui->weatherInfo->setText(respone.weacher());
        ui->windInfo->setText(respone.wind());
        ui->cityInfo->setText(respone.city());

        return true;
    }
    return false;
}


void HomePage::inithomepage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("QDialog{background-image: url(./images/homepage.png)}");
   //this->setStyleSheet("QDialog{background-image: url(:/page/images/main/homepage.png)}");
   //this->setStyleSheet("QPushButton {border:2px groove gray;border-radius:10px;padding:2px 4px;}");

    ui->timeInfo->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold; font-size:13px;color:rgb(0, 0, 0)");

    ui->weacherLabel->setStyleSheet("border-image: url(:/page/images/main/tianqi.png)");
    ui->weatherInfo->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:12px;color:rgb(0, 0, 0)");

    ui->windLabel->setStyleSheet("border-image: url(:/page/images/main/fengli.png)");
    ui->windInfo->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:12px;color:rgb(0, 0, 0)");

    ui->temperatureLabel->setStyleSheet("border-image: url(:/page/images/main/wendu.png)");
    ui->temperatureInfo->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:12px;color:rgb(0, 0, 0)");

    ui->cityLabel->setStyleSheet("border-image: url(:/page/images/main/chengshi.png)");
    ui->cityInfo->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:12px;color:rgb(0, 0, 0)");

    ui->storagePushButton->setFocusPolicy(Qt::NoFocus);
    ui->storagePushButton->setStyleSheet("background-color:transparent;background-image: url(:/page/images/main/cun.png)");

    ui->pickupPushButton->setFocusPolicy(Qt::NoFocus);
    ui->pickupPushButton->setStyleSheet("background-color:transparent;background-image: url(:/page/images/main/qu.png)");

    ui->resgistrationPushButton->setFocusPolicy(Qt::NoFocus);
    ui->resgistrationPushButton->setStyleSheet("background-color:transparent;background-image: url(:/page/images/main/register.png)");

    ui->rechangePushButton->setFocusPolicy(Qt::NoFocus);
    ui->rechangePushButton->setStyleSheet("background-color:transparent;background-image: url(:/page/images/main/recharge.png)");

    ui->blanceQueryPushButton->setFocusPolicy(Qt::NoFocus);
    ui->blanceQueryPushButton->setStyleSheet("background-color:transparent;background-image: url(:/page/images/main/check.png)");

    ui->exceHandPushButton->setFocusPolicy(Qt::NoFocus);
    ui->exceHandPushButton->setStyleSheet("background-color:transparent;background-image: url(:/page/images/main/handle.png)");

    ui->introductionPushButton->setFocusPolicy(Qt::NoFocus);
    ui->introductionPushButton->setStyleSheet("background-color:transparent;background-image: url(:/page/images/main/instruction.png)");

    ui->useProcessPushButton->setFocusPolicy(Qt::NoFocus);
    ui->useProcessPushButton->setStyleSheet("background-color:transparent;background-image: url(:/page/images/main/guide.png)");

    ui->priceCheckPushButton->setFocusPolicy(Qt::NoFocus);
    ui->priceCheckPushButton->setStyleSheet("background-color:transparent;background-image: url(:/page/images/main/tariff.png)");

    ui->setupPushButton->setFocusPolicy(Qt::NoFocus);
    ui->setupPushButton->setStyleSheet("background-color:transparent;background-image: url(:/page/images/main/set.png)");

    ui->small->setStyleSheet("border-image: url(:/page/images/main/xiao.png)");
    ui->smalllabel->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:15px;color:rgb(255, 0, 0);border-image: url(:/page/images/main/geshu.png)");

    ui->middle->setStyleSheet("border-image: url(:/page/images/main/zhong.png)");
    ui->middlelabel->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold; font-size:15px;color:rgb(255, 0, 0);border-image: url(:/page/images/main/geshu.png)");

    ui->big->setStyleSheet("border-image: url(:/page/images/main/da.png)");
    ui->biglabel->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:15px;color:rgb(255, 0, 0);border-image: url(:/page/images/main/geshu.png)");

    ui->vegetable->setStyleSheet("border-image: url(:/page/images/main/fresh.png)");
    ui->vegetablelabel->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:15px;color:rgb(255, 0, 0);border-image: url(:/page/images/main/geshu.png)");

    qsrand(time(0));
    mMinutes = qrand()%60;

    getWeather();

    weatherStatus = false;

    weekDayList.append(" 星期一 ");
    weekDayList.append(" 星期二 ");
    weekDayList.append(" 星期三 ");
    weekDayList.append(" 星期四 ");
    weekDayList.append(" 星期五 ");
    weekDayList.append(" 星期六 ");
    weekDayList.append(" 星期日 ");


//    ui->courier_toolbtn->setStyleSheet("background-color:transparent");
//    ui->user_toolbtn->setStyleSheet("background-color:transparent");
    //ui->courier_toolbtn->setEnabled(false);

/*
    QPixmap couriericon(":/images/courier.png");
    ui->courier_toolbtn->setIcon(couriericon);
    ui->courier_toolbtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->courier_toolbtn->setIconSize(couriericon.size());
    ui->courier_toolbtn->setText(tr("快递员存件"));
    ui->courier_toolbtn->setStyleSheet("background-color:transparent");

    QPixmap usericon(":/images/user.png");
    ui->user_toolbtn->setIcon(usericon);
    ui->user_toolbtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->user_toolbtn->setIconSize(usericon.size());
    ui->user_toolbtn->setText(tr("用户取件"));
    ui->user_toolbtn->setStyleSheet("background-color:transparent");
*/

}
void HomePage::setBtnEnable(bool status)
{
    ui->storagePushButton->setEnabled(status);
    ui->pickupPushButton->setEnabled(status);
    ui->rechangePushButton->setEnabled(status);
    ui->resgistrationPushButton->setEnabled(status);
    ui->priceCheckPushButton->setEnabled(status);
    ui->setupPushButton->setEnabled(status);
    ui->exceHandPushButton->setEnabled(status);
//    ui->cardCheckPushButton->setEnabled(status);
    ui->introductionPushButton->setEnabled(status);
    ui->useProcessPushButton->setEnabled(status);
}


void HomePage::showCurTime()
{
     QDateTime curTime = QDateTime::currentDateTime();

     QTime time = QTime::currentTime();
     int day = QDate::currentDate().dayOfWeek();

     QString str = curTime.toString("yyyy-MM-dd hh:mm:ss");

     int hour = time.hour();
     int minutes = time.minute();

     ui->timeInfo->setText(str + weekDayList.at(day - 1));

     if(!(hour%6))
     {
         if(!weatherStatus)
         {
             if(minutes == mMinutes)
             {
                weatherStatus = showWeather();
             }
         }
     }
     else
     {
         if(weatherStatus)
         {
            weatherStatus = false;
         }
     }
}

void HomePage::on_storagePushButton_clicked()//存件
{
    show_promptPage();
   // SCMainCtrl::instance()->enterStatus(ST_SET_PORT, "");
}
void HomePage::on_pickupPushButton_clicked()//取
{
    SCMainCtrl::instance()->enterStatus(ST_GET_DELIVERY, "");
}
void HomePage::on_rechangePushButton_clicked()//充值
{
    SCMainCtrl::instance()->enterStatus(ST_RECHARGE, "");
}
void HomePage::on_setupPushButton_clicked()//设置
{
    //show_promptPage();
    SCMainCtrl::instance()->enterStatus(ST_BIND_VIP, "");
}
void HomePage::on_resgistrationPushButton_clicked()//注册
{
    show_promptPage();
   // SCMainCtrl::instance()->enterStatus(ST_INFO_INPUT, "");
}
void HomePage::on_blanceQueryPushButton_clicked()//余额查询
{
    SCMainCtrl::instance()->enterStatus(ST_QUERY_BALANCE,"");
}
void HomePage::on_priceCheckPushButton_clicked()//资费查询
{
    SCMainCtrl::instance()->enterStatus(ST_PRICT_CHECK,"");
}
void HomePage::on_useProcessPushButton_clicked()//流程信息
{
    SCMainCtrl::instance()->enterStatus(ST_USE_GUIDE,"");
}
void HomePage::on_introductionPushButton_clicked()//易邮简介
{
    SCMainCtrl::instance()->enterStatus(ST_INTRO_LXYZ,"");
}
void HomePage::on_exceHandPushButton_clicked()//异常处理
{

    SCMainCtrl::instance()->enterStatus(ST_EXCEPTION_HANDLE,"");
}


void HomePage::upDataCellInfo()
{
    CellInfoDesc_t cellInfo;
    SCDatBase::instance()->getBoxCount(cellInfo);
    ui->biglabel->setText(QString("%1").arg(cellInfo.bigCell));
    ui->middlelabel->setText(QString("%1").arg(cellInfo.middleCell));
    ui->smalllabel->setText(QString("%1").arg(cellInfo.smallCell));
    ui->vegetablelabel->setText(QString("%1").arg(cellInfo.vegetableCell));
}



void HomePage::show_promptPage()
{
    promptInfo->showIcPromptPage();
}

void HomePage::close_promptPage()
{
    promptInfo->closeICPromptPage();
}

void HomePage::noicLog_clicked()
{
     SCMainCtrl::instance()->enterStatus(ST_NO_CARD_SAVE,"");
}

