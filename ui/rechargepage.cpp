#include "rechargepage.h"
#include "ui_rechargepage.h"
#include "servercomm.h"
#include "mainctrl.h"
#include "db.h"
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>

rechargepage::rechargepage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rechargepage)
{
    ui->setupUi(this);
    initPage();
}

rechargepage::~rechargepage()
{
    delete ui;
}

void rechargepage::initPage()
{
    isWorking = false;

    this->setObjectName("QWidget");
    this->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800,600);

    QLabel *prompt = new QLabel(this);
    prompt->resize(468,35);
    prompt->move(10,30);
    prompt->setStyleSheet("border-image:url(:/page/images/recharge/word1.png)");

    QLabel *pidCard = new QLabel(this);
    pidCard->resize(295,31);
    pidCard->move(50,155);
    pidCard->setStyleSheet("border-image:url(:/page/images/recharge/word2.png)");


    m1CardLineEdit = new CKeyBoardLineEdit(this);
    m1CardLineEdit->resize(350,40);
    m1CardLineEdit->move(350,153);
    m1CardLineEdit->installEventFilter(this);

    QLabel *idCard = new QLabel(this);
    idCard->resize(320,31);
    idCard->move(20,226);
    idCard->setStyleSheet("border-image:url(:/page/images/recharge/word3.png)");

    pwdLineEdit = new CKeyBoardLineEdit(this);
    pwdLineEdit->resize(350,40);
    pwdLineEdit->move(350,226);


    okBtn = new QPushButton(this);
    okBtn->resize(182,70);
    okBtn->move(170,494);
    okBtn->setFlat(true);
    okBtn->setFocusPolicy(Qt::NoFocus);
    okBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/okBtn.png)");


    cancelBtn = new QPushButton(this);
    cancelBtn->resize(182,70);
    cancelBtn->move(453,494);
    cancelBtn->setFlat(true);
    cancelBtn->setFocusPolicy(Qt::NoFocus);
    cancelBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/exitBtn.png)");

    connect(okBtn,SIGNAL(clicked()),this,SLOT(okbtn_slots()));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancelbtn_slots()));
    connect(&rechargeInfo,SIGNAL(rechargeInfo_Ok()),this,SLOT(commitInfo_slots()));
    connect(m1CardLineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));
    connect(pwdLineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));
}

void rechargepage::setBtnEnable(bool status)
{
    okBtn->setEnabled(status);
    cancelBtn->setEnabled(status);
}

void rechargepage::okbtn_slots()
{
    emit resetTime();

    setBtnEnable(false);
    QRegExp idrex("[0-9A-Za-z]\\w{9}");
    QRegExp pwdrex("[0-9A-Za-z]\\w{11}");

    QString idStr = m1CardLineEdit->text().trimmed();
    QString pwdStr = pwdLineEdit->text().trimmed();

    if(!idrex.exactMatch(idStr))
    {
        myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("请输入正确的卡号。"));
    }
    else if(!pwdrex.exactMatch(pwdStr))
    {
        myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("请输入正确的充值卡密码。"));
    }
    else
    {
        judgeStoredAccountResp respone;
        //接口部分，按照协议只需要传M1卡号以及充值卡密码

        SCServerComm::instance()->judgeStoredAccount(idStr,pwdStr,&respone);

        if(respone.success() == RET_FALSE)
        {          
            if(respone.errCode() == NOTEXIST_IC_ERROR)
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("充值卡号未注册。"));
            }
            else if(respone.errCode() == recharge_IC_ERROR)
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("充值帐户不存在。"));
            }
            else if(respone.errCode() == IC_FORBID_ERROR)
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("您的会员卡已被冻结，详情请咨询客服。"));
            }
            else if(respone.errCode() == recharge_PWD_ERROR)
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("充值密码错误。"));
            }
            else if(respone.errCode() == DEVICE_ERROR)
            {
                SCMainCtrl::instance()->enterStatus(ST_DEVICE_REST, "");
            }

        }
        else if(respone.success() == RET_TRUE)
        {
            topType = respone.cardType().toInt();

            if(0 == topType)
            {
                rechargeInfo.readTraiffInfo(respone.empName(),respone.mobilePhone(),respone.m1Card(),respone.price());
            }
            else
            {
                topDate = respone.topTimeEnd();
                rechargeInfo.readYearInfo(respone.empName(),respone.mobilePhone(),respone.m1Card(),respone.cardTimeEnd(),respone.topTimeEnd());
            }
        }
        else
        {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"));
        }
    }
    setBtnEnable(true);

}

void rechargepage::commitInfo_slots()
{
    emit resetTime();

    setBtnEnable(false);

    QString idStr = m1CardLineEdit->text().trimmed();
    QString pwdStr = pwdLineEdit->text().trimmed();


    rechargeToAccountResp respone;

    //确定充值接口
    SCServerComm::instance()->rechargeToAccount(idStr,pwdStr,&respone);


    if(respone.success() == RET_FALSE)
    {

       myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("充值失败，请联系客服。"));
    }
    else if(respone.success() == RET_TRUE)
    {
        if(0 == topType)
        {
            myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),\
                      QString("您已成功充值%1元，当前帐户余额%2元。").arg(respone.price()).arg(respone.balance()));
        }
        else
        {
            myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),\
                      QString("您已成功充值，免基础费用期至%1。").arg(topDate));
        }

        m1CardLineEdit->clear();
        pwdLineEdit->clear();
    }
    else
    {
         myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"));
    }

    setBtnEnable(true);
}

void rechargepage::cancelbtn_slots()
{
    isWorking = false;
    m1CardLineEdit->clear();
    pwdLineEdit->clear();
    SCMainCtrl::instance()->enterStatus(ST_HOME, "");
}


void rechargepage::initPageInfo()
{
    isWorking = false;
    m1CardLineEdit->clear();
    pwdLineEdit->clear();
    m1CardLineEdit->setFocus();
    topDate.clear();

}


void rechargepage::readCardInfo(QString data)
{
    if(isWorking)
    {
         m1CardLineEdit->setText(data);
         this->focusNextChild();
    }

}

bool rechargepage::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m1CardLineEdit)
    {
        if(event->type() == QEvent::FocusIn)
        {
            isWorking = true;
        }
        else if(event->type() == QEvent::FocusOut)
        {
            isWorking = false;
        }
    }

    return QDialog::eventFilter(obj,event);
}
