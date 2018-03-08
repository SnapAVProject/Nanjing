#include "financepage.h"
#include "servercomm.h"
#include "db.h"
#include "mainctrl.h"
#include <QWidget>
#include <QStackedLayout>
#include <QLabel>
#include <QPushButton>

//***************************************************************************
/**file		financepage.cpp
* brief		财务取款
* author	hjzhang@163.com	2014/3/18
* version	1.3
*/
//***************************************************************************

financepage::financepage(QWidget *parent) :
    QDialog(parent)
{

    this->setWindowFlags(Qt::FramelessWindowHint);
    stack = new QStackedLayout(this);

    initPage();
    initGetCodePage();

    this->resize(800,600);
    this->setLayout(stack);
}


void financepage::initPage()
{
    QWidget *takeCoinPage = new QWidget(this);

    takeCoinPage->setObjectName("QWidget");
    takeCoinPage->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");


    QLabel *prompt = new QLabel(takeCoinPage);
    prompt->resize(223,34);
    prompt->move(95,223);
    prompt->setStyleSheet("border-image :url(:/page/images/finance/word1.png)");

    codeLineEdit = new CKeyBoardLineEdit(takeCoinPage);
    codeLineEdit->resize(220,40);
    codeLineEdit->move(320,220);

    openBtn = new QPushButton(takeCoinPage);
    openBtn->resize(99,62);
    openBtn->move(560,205);
    openBtn->setFlat(true);
    openBtn->setFocusPolicy(Qt::NoFocus);
    openBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/take/okBtn.png)");

    getCodeBtn = new QPushButton(takeCoinPage);
    getCodeBtn->setFlat(true);
    getCodeBtn->resize(182,70);
    getCodeBtn->move(170,494);
    getCodeBtn->setFocusPolicy(Qt::NoFocus);
    getCodeBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/finance/getCodeBtn.png)");

    exitBtn = new QPushButton(takeCoinPage);
    exitBtn->setFlat(true);
    exitBtn->resize(182,70);
    exitBtn->move(453,494);
    exitBtn->setFocusPolicy(Qt::NoFocus);
    exitBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/exitBtn.png)");

    stack->addWidget(takeCoinPage);

    connect(openBtn,SIGNAL(clicked()),this,SLOT(openBankCell()));
    connect(getCodeBtn,SIGNAL(clicked()),this,SLOT(skipGetCode()));
    connect(exitBtn,SIGNAL(clicked()),this,SLOT(quitPage()));

    connect(codeLineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));
    connect(codeLineEdit,SIGNAL(okBtnPress()),this,SLOT(openBankCell()));
}


void financepage::initGetCodePage()
{
    QWidget *getCodePage = new QWidget(this);

    getCodePage->setObjectName("QWidget");
    getCodePage->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");

    QLabel *prompt = new QLabel(getCodePage);
    prompt->resize(606,48);
    prompt->move(30,30);
    prompt->setStyleSheet("border-image: url(:/page/images/finance/word2.png)");

    QLabel *label = new QLabel(getCodePage);
    label->resize(135,32);
    label->move(150,223);
    label->setStyleSheet("border-image: url(:/page/images/take/word4.png)");


    phoneLineEdit = new CKeyBoardLineEdit(getCodePage);
    phoneLineEdit->resize(220,40);
    phoneLineEdit->move(320,220);

    okBtn = new QPushButton(getCodePage);
    okBtn->resize(182,70);
    okBtn->move(170,494);
    okBtn->setFlat(true);
    okBtn->setFocusPolicy(Qt::NoFocus);
    okBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/okBtn.png)");


    cancelBtn = new QPushButton(getCodePage);
    cancelBtn->resize(182,70);
    cancelBtn->move(453,494);
    cancelBtn->setFlat(true);
    cancelBtn->setFocusPolicy(Qt::NoFocus);
    cancelBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/cancelBtn.png)");

    stack->addWidget(getCodePage);

    connect(okBtn,SIGNAL(clicked()),this,SLOT(getCode()));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(quitGetCode()));
    connect(phoneLineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));
    connect(phoneLineEdit,SIGNAL(okBtnPress()),this,SLOT(getCode()));


}


void financepage::readInfo(QString info)
{
    isWorking = false;
    m1Card = info;
    stack->setCurrentIndex(0);
}

void financepage::setBtnEnable(bool status)
{
    this->openBtn->setEnabled(status);
    this->getCodeBtn->setEnabled(status);
    this->exitBtn->setEnabled(status);
    this->okBtn->setEnabled(status);
    this->cancelBtn->setEnabled(status);
}

void financepage::skipGetCode()
{
    emit resetTime();
    this->codeLineEdit->clear();
    stack->setCurrentIndex(1);
}

void financepage::quitPage()
{
    this->codeLineEdit->clear();
    this->phoneLineEdit->clear();
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

bool financepage::judgeCell()
{
    QList<ServerDesc_t> serverlist;
    SCDatBase::instance()->queryServerException(serverlist);

    for(int i = 0;i < serverlist.size(); i++)
    {
        if(serverlist.at(i).errType == "0")    //取款箱门打不开类型
        {
            return true;
        }
    }

    return false;
}

void financepage::openBankCell()
{
    emit resetTime();

    setBtnEnable(false);

    if(!isWorking)
    {
        isWorking = true;
        if(judgeCell())
        {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("钱箱打不开"));
        }
        else
        {
            startOpencell();
        }
        isWorking = false;
    }

    setBtnEnable(true);

}

void financepage::startOpencell()
{
    QString state;
    judgePwdResp respone;

    QString code = this->codeLineEdit->text().trimmed();

    SCServerComm::instance()->judgePwd(code,&respone);
    if(respone.success() == RET_TRUE)
    {
        qint8 cabinetId = 1;  //北京0120
        qint8 cellId = 20;
        QString coinCount = respone.money();
        if(lockoperation.openLock(cabinetId,cellId))
        {
            state = "2"; //打开取钱罐
            SCServerComm::instance()->takeRecord(m1Card,coinCount,state);
        }
        else
        {
            state = "1"; //取钱罐打不开
            SCServerComm::instance()->takeRecord(m1Card,coinCount,state);

            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("钱箱打不开"));
        }
    }
    else if(respone.success() == "false" && respone.errCode() == TAKE_MONEY_ERROR)
    {
         myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("钱箱打不开"));
    }
    else if(respone.success() == "false" && respone.errCode() == "10024")
    {
         myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("取款码错误"));
    }
    else
    {
         myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"));
    }

    this->codeLineEdit->clear();

}

void financepage::getCode()
{
    emit resetTime();
    setBtnEnable(false);

    if(!isWorking)
    {
        isWorking = true;
        QRegExp rxcellphone("^1\\d{10}");
        if (rxcellphone.exactMatch(this->phoneLineEdit->text().trimmed()))
        {
            //通知后台发送短信给此手机号码
            getRanCodeAgainResp respone;
            SCServerComm::instance()->financeGetPwd(phoneLineEdit->text().trimmed(),&respone);

            if(respone.success() == "true")
            {
               myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),tr("短信已发送请注意查收。"));
               stack->setCurrentIndex(0);

            }
            else if(respone.success() == "false")
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("请输入正确的手机号码。"));
            }
            else
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"));
            }
        }
        else
        {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("手机号码输入有误,请输入正确号码。"));

        }

        this->phoneLineEdit->clear();
        isWorking = false;
    }
    setBtnEnable(true);
}

void financepage::quitGetCode()
{
    this->phoneLineEdit->clear();
    stack->setCurrentIndex(0);
}
