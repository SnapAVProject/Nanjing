#include "crechargeinfo.h"
#include "servercomm.h"
#include "db.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QLayout>
#include <QBitmap>
#include <QTimer>

CRechargeInfo::CRechargeInfo(QWidget *parent) :
    QDialog(parent)
{
    initPage();
}


void CRechargeInfo::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    QPixmap pixmap(":/page/images/recharge/beijing.png");
    this->setMask(pixmap.mask());
    this->resize( pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);
    this->resize(480,450);
    this->move(160,140);

    timer = new QTimer(this);
    timer->setInterval(30000);
    timer->setSingleShot(true);

    QLabel *prompt = new QLabel(this);
    prompt->resize(341,33);
    prompt->move(10,10);
    prompt->setStyleSheet("border-image:url(:/page/images/recharge/word4.png)");

    QLabel *nameLabel = new QLabel(this);
    nameLabel->resize(59,25);
    nameLabel->move(120,103);
    nameLabel->setStyleSheet("border-image:url(:/page/images/recharge/word5.png)");

    mNameLineEdit = new QLineEdit(this);
    mNameLineEdit->setReadOnly(true);
    mNameLineEdit->resize(220,30);
    mNameLineEdit->move(200,100);
    mNameLineEdit->setStyleSheet("font-family:wenquanyi; font-size:18px;");

    QLabel *phoneLabel = new QLabel(this);
    phoneLabel->resize(109,25);
    phoneLabel->move(70,150);
    phoneLabel->setStyleSheet("border-image:url(:/page/images/recharge/word6.png)");

    mMobilePhoneLineEdit = new QLineEdit(this);
    mMobilePhoneLineEdit->setReadOnly(true);
    mMobilePhoneLineEdit->resize(220,30);
    mMobilePhoneLineEdit->move(200,150);
    mMobilePhoneLineEdit->setStyleSheet("font-family:wenquanyi; font-size:18px;");

    QLabel *M1CardLabel = new QLabel(this);
    M1CardLabel->resize(128,25);
    M1CardLabel->move(50,200);
    M1CardLabel->setStyleSheet("border-image:url(:/page/images/recharge/word7.png)");


    mM1CardLineEdit = new QLineEdit(this);
    mM1CardLineEdit->setReadOnly(true);
    mM1CardLineEdit->resize(220,30);
    mM1CardLineEdit->move(200,200);
    mM1CardLineEdit->setStyleSheet("font-family:wenquanyi; font-size:18px;");

    tariffFrame = new QFrame(this);
    tariffFrame->resize(480,100);
    tariffFrame->move(0,250);

    QLabel *priceLabel = new QLabel(tariffFrame);
    priceLabel->resize(150,25);
    priceLabel->move(30,0);
    priceLabel->setStyleSheet("border-image:url(:/page/images/recharge/word8.png)");

    mPriceLineEdit = new QLineEdit(tariffFrame);
    mPriceLineEdit->setReadOnly(true);
    mPriceLineEdit->resize(220,30);
    mPriceLineEdit->move(200,0);
    mPriceLineEdit->setStyleSheet("font-family:wenquanyi; font-size:18px;");


    yearFrame = new QFrame(this);
    yearFrame->resize(480,100);
    yearFrame->move(0,250);

    QLabel *dataLabel = new QLabel(yearFrame);
    dataLabel->resize(134,25);
    dataLabel->move(50,0);
    dataLabel->setStyleSheet("border-image:url(:/page/images/recharge/word9.png)");

    mCardDateLineEdit = new QLineEdit(yearFrame);
    mCardDateLineEdit->setReadOnly(true);
    mCardDateLineEdit->resize(220,30);
    mCardDateLineEdit->move(200,0);
    mCardDateLineEdit->setStyleSheet("font-family:wenquanyi; font-size:18px;");

    QLabel *topDataLabel = new QLabel(yearFrame);
    topDataLabel->resize(156,25);
    topDataLabel->move(30,50);
    topDataLabel->setStyleSheet("border-image:url(:/page/images/recharge/word10.png)");

    mTopDateLineEdit = new QLineEdit(yearFrame);
    mTopDateLineEdit->setReadOnly(true);
    mTopDateLineEdit->resize(220,30);
    mTopDateLineEdit->move(200,50);
    mTopDateLineEdit->setStyleSheet("font-family:wenquanyi; font-size:18px;");

    QPushButton *okBtn = new QPushButton(this);
    okBtn->resize(117,45);
    okBtn->move(100,360);
    okBtn->setFlat(true);
    okBtn->setFocusPolicy(Qt::NoFocus);
    okBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/okBtn_p.png)");


    QPushButton *cancelBtn = new QPushButton(this);
    cancelBtn->resize(117,45);
    cancelBtn->move(250,360);
    cancelBtn->setFlat(true);
    cancelBtn->setFocusPolicy(Qt::NoFocus);
    cancelBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/cancelBtn_p.png)");



    connect(okBtn,SIGNAL(clicked()),this,SLOT(closeRechargePage()));
    connect(okBtn,SIGNAL(clicked()),this,SIGNAL(rechargeInfo_Ok()));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(closeRechargePage()));
    connect(timer,SIGNAL(timeout()),this,SLOT(closeRechargePage()));

}

void CRechargeInfo::readYearInfo(QString name, QString mobilePhone,QString m1Card, QString first,QString second)
{
    mNameLineEdit->setText(name);
    mMobilePhoneLineEdit->setText(mobilePhone);
    mM1CardLineEdit->setText(m1Card);
    mCardDateLineEdit->setText(first);
    mTopDateLineEdit->setText(second);
    yearFrame->show();
    tariffFrame->hide();
    timer->start();
    this->exec();
}


void CRechargeInfo::readTraiffInfo(QString name, QString mobilePhone, QString m1Card, QString price)
{
    mNameLineEdit->setText(name);
    mMobilePhoneLineEdit->setText(mobilePhone);
    mM1CardLineEdit->setText(m1Card);
    mPriceLineEdit->setText(price);
    yearFrame->hide();
    tariffFrame->show();
    timer->start();
    this->exec();
}

void CRechargeInfo::closeRechargePage()
{
    timer->stop();
    mNameLineEdit->clear();
    mMobilePhoneLineEdit->clear();
    mCardDateLineEdit->clear();
    mM1CardLineEdit->clear();
    mTopDateLineEdit->clear();

    this->close();
}




