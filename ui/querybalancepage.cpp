#include "querybalancepage.h"
#include "ui_querybalancepage.h"
#include "mainctrl.h"
#include "servercomm.h"
#include "db.h"
#include <QLabel>
#include <QPushButton>

querybalancepage::querybalancepage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::querybalancepage)
{
    ui->setupUi(this);
    initPage();
}

querybalancepage::~querybalancepage()
{
    delete ui;
}


void querybalancepage::initPage()
{
    this->setObjectName("QWidget");
    this->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800,600);

    QLabel *prompt = new QLabel(this);
    prompt->resize(464,41);
    prompt->move(10,30);
    prompt->setStyleSheet("border-image: url(:/page/images/querybalance/word1.png)");

    QLabel *idCard = new QLabel(this);
    idCard->resize(287,39);
    idCard->move(30,206);
    idCard->setStyleSheet("border-image: url(:/page/images/querybalance/word2.png)");

    idLineEdit = new CKeyBoardLineEdit(this);
    idLineEdit->resize(350,40);
    idLineEdit->move(330,206);

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

    connect(okBtn,SIGNAL(clicked()),this,SLOT(okBtn_slots()));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtn_slots()));
    connect(idLineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));
}

void querybalancepage::cancelBtn_slots()
{
    this->idLineEdit->clear();

    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}


void querybalancepage::okBtn_slots()
{
    emit resetTime();
    checkBalanceResp respone;
    okBtn->setEnabled(false);
    cancelBtn->setEnabled(false);

    QString idStr = idLineEdit->text().trimmed();
    if( !idStr.isEmpty())
    {
        SCServerComm::instance()->checkBalance(idStr,&respone);
        if(respone.success() == RET_TRUE)
        {
            idLineEdit->clear();
            myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),QString("您当前帐户余额%1元，免基础费用至%2。").arg(respone.balance()).arg(respone.cardTimeEnd()));
        }
        else if(respone.success() == RET_FALSE && respone.errCode() == NOTEXIST_IC_ERROR)
        {
             myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("您输入的卡号不存在。"));
        }
        else if(respone.success() == RET_FALSE && respone.errCode() == recharge_IC_ERROR)
        {
             myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("账户不存在。"));
        }
        else
        {
             myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"));
        }
    }
    else
    {
        myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("请输入正确的卡号。"));
    }

    okBtn->setEnabled(true);
    cancelBtn->setEnabled(true);
}

void querybalancepage::readCardInfo(QString data)
{
    idLineEdit->setText(data);
}
