#include "debtpage.h"
#include "ui_debtpage.h"
#include "db.h"
#include <QBitmap>

CDebtPage::CDebtPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::debtpage)
{
    ui->setupUi(this);

    timer.setInterval(120*1000);
    timer.setSingleShot(true);

    myMessageBox = new CMessageBox(3*1000);

    initPage();

    connect(&timer,SIGNAL(timeout()),this,SLOT(timeOver()));
}

CDebtPage::~CDebtPage()
{
    delete ui;
    deleteLater();
}


void CDebtPage::initPage()
{
    QPixmap pixmap(":/page/images/debt/debt.png");
    this->setMask(pixmap.mask());
    this->resize( pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(480,450);
    this->move(160,140);


    vipPercentLab = new QLabel(this);
    vipPercentLab->resize(50,40);
    vipPercentLab->move(93,95);
    vipPercentLab->setAlignment(Qt::AlignCenter);
    vipPercentLab->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:20px;color:rgb(255,255,255)");

    delOwnerCostLab = new QLabel(this);
    delOwnerCostLab->resize(80,30);
    delOwnerCostLab->move(260,203);
    delOwnerCostLab->setAlignment(Qt::AlignRight);
    delOwnerCostLab->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:18px;color:rgb(255,255,255)");

    debtCostLab = new QLabel(this);
    debtCostLab->resize(80,30);
    debtCostLab->move(260,235);
    debtCostLab->setAlignment(Qt::AlignRight);
    debtCostLab->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:18px;color:rgb(255,255,255)");

    ownerHasPayLab = new QLabel(this);
    ownerHasPayLab->resize(80,30);
    ownerHasPayLab->move(260,270);
    ownerHasPayLab->setAlignment(Qt::AlignRight);
    ownerHasPayLab->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:18px;color:rgb(0,255,0)");

    coinLab = new QLabel(this);
    coinLab->resize(60,45);
    coinLab->move(250,320);
    coinLab->setAlignment(Qt::AlignCenter);
    coinLab->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:26px;color:rgb(255,0,128)");

    QLabel *coin = new QLabel(this);
    coin->resize(35,35);
    coin->move(310,325);
    coin->setStyleSheet("border-image:url(:/page/images/debt/debtCoin.png)");


    quitBtn = new QPushButton(this);
    quitBtn->resize(115,45);
    quitBtn->move(180,400);
    quitBtn->setFlat(true);
    quitBtn->setFocusPolicy(Qt::NoFocus);
    quitBtn->setStyleSheet("background-color:transparent;background-image: url(:/page/images/debt/exitBtn.png)");




    connect(quitBtn,SIGNAL(clicked()),this,SLOT(quitDebtPage()));
    connect(myMessageBox,SIGNAL(message_ok()),this,SLOT(retBtnStatus()));
    connect(myMessageBox,SIGNAL(message_cancel()),this,SLOT(retBtnStatus()));
    connect(myMessageBox,SIGNAL(message_close()),this,SLOT(retBtnStatus()));

}


void CDebtPage::setCoinLab(QString data)
{
    timer.setInterval(120*1000);
    this->coinLab->setText(data);

    qint8 payMoney = ownerHasPayLab->text().split(" ").at(0).toInt();

    ownerHasPayLab->setText(QString("%1 元").arg(++payMoney));
    coinNumber++;
}


void CDebtPage::updateCoinLab(QString data)
{
    timer.setInterval(120*1000);
    delOwnerCostLab->setText("0");
    this->coinLab->setText(data);
}

void CDebtPage::quitDebtPage()
{
  //  emit debtPageClose();
    timer.stop();
    handleDebtInfo();
    this->close();
}

void CDebtPage::debtTimeStart(QString data,QString vip,double demurrage,double baseCost,double ownerHasPay)
{
    deliveryId = data;
    coinNumber = 0;

    if("1" == vip.split(".").at(0))
    {
        vipPercentLab->setText("不打");
    }
    else if(vip.split(".").size() > 1)
    {
        QString strVip = vip.split(".").at(1);
        if(vip.split(".").at(1).size() > 1)
        {
            strVip.insert(1,QString("."));
        }
        vipPercentLab->setText(strVip);
    }
    else
    {
        vipPercentLab->setText(vip.split(".").at(0));
    }
    debtCostLab->setText(QString("%1 元").arg(demurrage));
    delOwnerCostLab->setText(QString("%1 元").arg(baseCost));
    ownerHasPayLab->setText(QString("%1 元").arg(ownerHasPay));
    timer.start();
    this->show();
}

void CDebtPage::timeOver()
{
 //   emit debtPageClose();
    handleDebtInfo();
    this->close();
}

void CDebtPage::handleDebtInfo()
{
    bool status = true;
    if(coinNumber != 0)
    {
        QList<DebtDesc_t> debtlist;
        SCDatBase::instance()->queryDebtException(debtlist);
        for(int i = 0;i < debtlist.size();i++)
        {
            if(debtlist.at(i).deliveryId == deliveryId)
            {
                qint8 debt = debtlist.at(i).debt + coinNumber;
                SCDatBase::instance()->setDebtException(debtlist.at(i).index,debt);
                status = false;
                break;
            }
        }

        if(status)
        {
            SCDatBase::instance()->insertDebtException(deliveryId,coinNumber);
        }
    }

}

void CDebtPage::closeCurDebtPage()
{
  //  emit debtPageClose();
    timer.stop();
    this->close();
}

void CDebtPage::updateDebtInfo(QString data)
{
    this->coinLab->setText(data);
}

void CDebtPage::setBtnEnable(bool status)
{
    quitBtn->setEnabled(status);
}


void CDebtPage::promptInfo(QMessageBox::Icon icon, const QString &title, const QString &text,Qt::WindowModality model)
{
    quitBtn->setEnabled(false);
    myMessageBox->myPrompt(icon,title,text,model);
}

void CDebtPage::retBtnStatus()
{
    quitBtn->setEnabled(true);
}



void CDebtPage::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


