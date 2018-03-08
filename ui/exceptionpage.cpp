#include "exceptionpage.h"
#include "ui_exceptionpage.h"
#include "mainctrl.h"
#include "servercomm.h"
#include <QPushButton>
#include <QLabel>

exceptionpage::exceptionpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::exceptionpage)
{
    ui->setupUi(this);
    myMessageBox = new CMessageBox(2*1000);
    promptInfo = new CPromptInfo();
    initPage();
}

exceptionpage::~exceptionpage()
{
    delete ui;
}


void exceptionpage::initPage()
{
    this->setObjectName("QWidget");
    this->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800,600);

    QLabel *prompt = new QLabel(this);
    prompt->resize(744,102);
    prompt->move(30,30);
    prompt->setStyleSheet("border-image: url(:/page/images/exception/word.png)");

    openDoorBtn = new QPushButton(this);
    openDoorBtn->setFixedSize(182,70);
    openDoorBtn->move(50,300);
    openDoorBtn->setFlat(true);
    openDoorBtn->setFocusPolicy(Qt::NoFocus);
    openDoorBtn->setStyleSheet("background-color:transparent;background-image: url(:/page/images/exception/emptyDoorBtn.png)");


    delayPackageBtn = new QPushButton(this);
    delayPackageBtn->setFixedSize(182,70);
    delayPackageBtn->move(300,300);
    delayPackageBtn->setFlat(true);
    delayPackageBtn->setFocusPolicy(Qt::NoFocus);
    delayPackageBtn->setStyleSheet("background-color:transparent;background-image: url(:/page/images/exception/delayBtn.png)");


    courierBtn = new QPushButton(this);
    courierBtn->setFixedSize(180,70);
    courierBtn->move(550,300);
    courierBtn->setFlat(true);
    courierBtn->setFocusPolicy(Qt::NoFocus);
    courierBtn->setStyleSheet("background-color:transparent;background-image: url(:/page/images/exception/courierTakeBtn.png)");



    exitBtn = new QPushButton(this);
    exitBtn->setFixedSize(182,70);
    exitBtn->move(320,480);
    exitBtn->setFlat(true);
    exitBtn->setFocusPolicy(Qt::NoFocus);
    exitBtn->setStyleSheet("background-color:transparent;background-image: url(:/public/images/public/exitBtn.png)");


    WorkingStatus = 0;

    connect(openDoorBtn,SIGNAL(clicked()),this,SLOT(openDoor_slots()));
    connect(delayPackageBtn,SIGNAL(clicked()),this,SLOT(takeDelayPackage()));
    connect(courierBtn,SIGNAL(clicked()),this,SLOT(courier_slots()));
    connect(exitBtn,SIGNAL(clicked()),this,SLOT(exit_slots()));
    connect(promptInfo,SIGNAL(logRequest()),SLOT(noCardLogin()));


}

void exceptionpage::openDoor_slots()
{
    emit resetTime();
    WorkingStatus = 1;
    promptInfo->showIcPromptPage();
   // myMessageBox->myPrompt(QMessageBox::Information,tr("提示"),tr("请刷您的会员卡。"));
}


void exceptionpage::takeDelayPackage()
{
    emit resetTime();
    WorkingStatus = 2;
    promptInfo->showIcPromptPage();
    //myMessageBox->myPrompt(QMessageBox::Information,tr("提示"),tr("请刷您的会员卡。"));
}

void exceptionpage::courier_slots()
{
    emit resetTime();
    WorkingStatus = 3;
    promptInfo->showIcPromptPage();
}

void exceptionpage::exit_slots()
{
    WorkingStatus = 0;
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

void exceptionpage::setBtnEnable(bool status)
{
    openDoorBtn->setEnabled(status);
    exitBtn->setEnabled(status);
    delayPackageBtn->setEnabled(status);
    courierBtn->setEnabled(status);
}


void exceptionpage::noCardLogin()
{
    QString str;
    str.clear();
    if(1 == WorkingStatus)
    {
        str = "STATUS1";
    }
    else if(2 == WorkingStatus)
    {
        str = "STATUS2";
    }
    else if(3 == WorkingStatus)
    {
        str = "STATUS3";
    }

    WorkingStatus = 0;
    SCMainCtrl::instance()->enterStatus(ST_NO_CARD_SAVE,str.toAscii());
}

void exceptionpage::recvCardInfo(QString data)
{
    promptInfo->closeICPromptPage();

    progress.setMinimum(0);
    progress.setMaximum(0);
    progress.setValue(0);
    progress.setCancelButton(0);
    progress.setWindowFlags(Qt::FramelessWindowHint);
    progress.setLabelText(tr("处理中，请稍后..."));
    progress.show();

    if(1 == WorkingStatus)
    {
        WorkingStatus = 0;
        checkOpenScellResp respone;

        setBtnEnable(false);
        SCServerComm::instance()->checkOpenScell(data,&respone);
        setBtnEnable(true);

        progress.hide();

        if(respone.errCode() == "0000") //卡类型符合空箱处理
        {
            SCMainCtrl::instance()->enterStatus(ST_EXCEPTION_OPENDOOR,data.toAscii());
        }
        else if(!respone.errCode().isEmpty())
        {
             myMessageBox->myPrompt(QMessageBox::Information,tr("提示"),respone.errMessage());
        }
        else
        {
            myMessageBox->myPrompt(QMessageBox::Information,tr("提示"),tr("网络异常,请稍后再试。"));
        }
    }
    else if(2 == WorkingStatus)
    {
        WorkingStatus = 0;

        QByteArray ret;
        deliveryTimeoutResp respone;
        QString mobilePhone = "";

        setBtnEnable(false);
        ret = SCServerComm::instance()->deliveryTimeout(data,mobilePhone,&respone);
        setBtnEnable(true);
        progress.hide();

        if(respone.errCode() == "0000" && respone.getDeliveryInfoList().size() > 0)
        {
            SCMainCtrl::instance()->enterStatus(ST_COURIER_TAKE,ret);
        }
        else if(respone.errCode() == "0000")
        {
             myMessageBox->myPrompt(QMessageBox::Information,tr("提示"),tr("您没有滞留件需要处理。"));
        }
        else if(!respone.errCode().isEmpty())
        {
             myMessageBox->myPrompt(QMessageBox::Information,tr("提示"),respone.errMessage());
        }
        else
        {
            myMessageBox->myPrompt(QMessageBox::Information,tr("提示"),tr("网络异常,请稍后再试。"));
        }

    }

    else if( 3 == WorkingStatus)
    {
        WorkingStatus = 0;
        QByteArray ret;
        expGetSaveDeliveryResp respone;
        QString passwd;
        setBtnEnable(false);
        ret = SCServerComm::instance()->expGetSaveDelivery(data,passwd,&respone);
        setBtnEnable(true);
        progress.hide();

        if(respone.errCode() == "0000" && respone.getDeliveryInfoList().size() > 0)
        {
            SCMainCtrl::instance()->enterStatus(ST_COURIER_HELP_TAKE,ret);
        }
        else if(respone.errCode() == "0000")
        {
             myMessageBox->myPrompt(QMessageBox::Information,tr("提示"),tr("该终端没有您存的快件。"));
        }
        else if(!respone.errCode().isEmpty())
        {
             myMessageBox->myPrompt(QMessageBox::Information,tr("提示"),respone.errMessage());
        }
        else
        {
            myMessageBox->myPrompt(QMessageBox::Information,tr("提示"),tr("网络异常,请稍后再试。"));
        }
    }
}

qint8 exceptionpage::getIsWorking()
{
    return WorkingStatus;
}
