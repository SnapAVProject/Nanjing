#include "vipbindpage.h"
#include "servercomm.h"
#include "mainctrl.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>


//***************************************************************************
/**file		vipbindpage.cpp
* brief		会员自助绑定
* author	hjzhang@163.com	2014/4/22
* version	1.3
*/
//***************************************************************************

vipbindpage::vipbindpage(QWidget *parent) :
    QDialog(parent)
{
    initPage();
}


void vipbindpage::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("QDialog{background-image: url(:/public/images/public/beijing.png)}");

    this->resize(800,600);

    QLabel *label = new QLabel(this);
    label->resize(706,99);
    label->move(30,0);
    label->setStyleSheet("border-image:url(:/page/images/bind/word1.png)");

    QLabel *m1Card = new QLabel(this);
    m1Card->resize(200,34);
    m1Card->move(80,110);
    m1Card->setStyleSheet("border-image:url(:/page/images/bind/word2.png)");

    QLabel *phone = new QLabel(this);
    phone->resize(141,34);
    phone->move(135,180);
    phone->setStyleSheet("border-image:url(:/page/images/bind/word3.png)");

    QLabel *code = new QLabel(this);
    code->resize(141,34);
    code->move(135,250);
    code->setStyleSheet("border-image:url(:/page/images/bind/word4.png)");

    m1Card_lineEdit = new QLineEdit(this);
    m1Card_lineEdit->resize(300,40);
    m1Card_lineEdit->move(330,110);
    m1Card_lineEdit->setStyleSheet("QLineEdit{font-family:wenquanyi; font-size:30px;padding: 1px;border-style: solid;border: 2px solid gray;border-radius: 8px;}");

    phone_lineEdit = new CKeyBoardLineEdit(this);
    phone_lineEdit->resize(300,40);
    phone_lineEdit->move(330,180);

    code_lineEdit = new CKeyBoardLineEdit(this);
    code_lineEdit->resize(300,40);
    code_lineEdit->move(330,250);

    okBtn = new QPushButton(this);
    okBtn->resize(182,70);
    okBtn->move(170,494);
    okBtn->setFlat(true);
    okBtn->setFocusPolicy(Qt::NoFocus);
    okBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/okBtn.png)");

    quitBtn = new QPushButton(this);
    quitBtn->setFlat(true);
    quitBtn->resize(182,70);
    quitBtn->move(453,494);
    quitBtn->setFocusPolicy(Qt::NoFocus);
    quitBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/exitBtn.png)");

    connect(okBtn,SIGNAL(clicked()),this,SLOT(start_bind()));
    connect(quitBtn,SIGNAL(clicked()),this,SLOT(closePage()));
}

void vipbindpage::setM1Card(QString data)
{
    this->m1Card_lineEdit->setText(data);
}

bool vipbindpage::judgeContent()
{
    QRegExp rxCellPhone("^1\\d{10}");
    bool ret = true;
    QString info;

    if(!rxCellPhone.exactMatch(phone_lineEdit->text().trimmed()))
    {
        ret = false;
        info = "请输入正确的手机号码。";
    }
    else if(m1Card_lineEdit->text().trimmed().isEmpty())
    {
        ret = false;
        info = "请刷您的IC卡。";
    }
    else if(code_lineEdit->text().trimmed().isEmpty())
    {
        ret = false;
        info = "请输入您的绑定密码。";
    }

    if(!ret)
    {
        myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"), info);
    }

     return ret;

}

void vipbindpage::start_bind()
{
    resetTime();
    okBtn->setEnabled(false);
    quitBtn->setEnabled(false);


    if(judgeContent())
    {
        QString m1card = m1Card_lineEdit->text().trimmed();
        QString phone = phone_lineEdit->text().trimmed();
        QString code = code_lineEdit->text().trimmed();

        VipBindingM1cardResp respone;
        SCServerComm::instance()->VipBindingM1card(m1card,phone,code,&respone);

        if(respone.success() == RET_TRUE)
        {
            this->m1Card_lineEdit->clear();
            this->phone_lineEdit->clear();
            this->code_lineEdit->clear();
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"), tr("绑定成功。"));
        }
        else if(respone.success() == RET_FALSE)
        {
            if(respone.errCode() == EXIST_CUSTOMER)
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"), tr("用户已注册。"));
            }
            else if(respone.errCode() == VIP_BIND_PHONE)
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"), tr("该手机号未开通自助会员功能。"));
            }
            else if(respone.errCode() == VIP_BIND_CODE)
            {
                 myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"), tr("绑定密码错误。"));
            }
        }
        else
        {
             myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"));
        }

    }

    okBtn->setEnabled(true);
    quitBtn->setEnabled(true);

}

void vipbindpage::closePage()
{
    this->m1Card_lineEdit->setFocus();
    this->m1Card_lineEdit->clear();
    this->phone_lineEdit->clear();
    this->code_lineEdit->clear();

    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
