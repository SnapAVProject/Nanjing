#include "cardtakeinfo.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QBitmap>
#include <QLabel>

cardtakeinfo::cardtakeinfo(QWidget *parent) :
    QDialog(parent)
{
    initPage();

    timer.setInterval(5*1000);
    timer.setSingleShot(true);

    connect(&timer,SIGNAL(timeout()),this,SLOT(timerOver()));
}


void cardtakeinfo::initPage()
{
    setWindowFlags(Qt::FramelessWindowHint);
    this->resize(400,250);
    this->move((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);

    QPixmap pixmap(":/page/images/prompt/prompt.png");
    this->setMask(pixmap.mask());
    this->resize( pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);

    takeInfoLabel = new QLabel(this);
    takeInfoLabel->resize(400,100);
    takeInfoLabel->move(0,50);
    takeInfoLabel->setAlignment(Qt::AlignCenter);
    takeInfoLabel->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:20px; color: rgb(255, 255, 255)");

    payLabel = new QLabel(this);
    payLabel->resize(400,40);
    payLabel->move(0,200);
    payLabel->setAlignment(Qt::AlignCenter);
    payLabel->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:18px; color: rgb(255, 255, 0)");

}

void cardtakeinfo::setInfo(double open, double close, double pay, double balance,bool role)
{
    takeInfoLabel->setText(QString("打开%1个箱门，打不开箱门%2个。").arg(open).arg(close));
    payLabel->setText(QString("刷卡支付%1元，帐户余额%2元。").arg(pay).arg(balance-pay));

    if(role)
    {
        payLabel->show();
    }
    else
    {
        payLabel->hide();
    }

    timer.start();
    this->show();
}

void cardtakeinfo::timerOver()
{
    emit closeInfoPage();
    this->close();
}
