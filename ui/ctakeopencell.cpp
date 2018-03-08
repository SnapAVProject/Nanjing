#include "ctakeopencell.h"
#include <QHBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QBitmap>
#include <QDebug>

CTakeOpenCell::CTakeOpenCell(QWidget *parent) :
    QDialog(parent)
{
    initPage();

    timer.setInterval(10*1000);
    timer.setSingleShot(true);

    connect(&timer,SIGNAL(timeout()),this,SLOT(timerOver()));
}


void CTakeOpenCell::initPage()
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


    QLabel *door = new QLabel(this);
    door->setFixedSize(86,45);
    door->move(75,105);
    door->setStyleSheet("background-image: url(:/page/images/saveWait/door.png)");


    doorId = new QLabel(this);
    doorId->setAlignment(Qt::AlignCenter);
    doorId->resize(40,40);
    doorId->move(170,108);
    doorId->setStyleSheet("font-family:wenquanyi; font-size:30px; color: rgb(0, 255, 64)");

    QLabel *doorCabinet = new QLabel(this);
    doorCabinet->resize(37,40);
    doorCabinet->move(220,108);
    doorCabinet->setStyleSheet("border-image: url(:/page/images/saveWait/doorcabinet.png)");

    doorCellId = new QLabel(this);
    doorCellId->resize(40,40);
    doorCellId->move(270,108);
    doorCellId->setStyleSheet("font-family:wenquanyi; font-size:30px; color: rgb(0, 255, 64)");

    QLabel *doorCell = new QLabel(this);
    doorCell->resize(37,40);
    doorCell->move(315,108);
    doorCell->setStyleSheet("border-image: url(:/page/images/saveWait/doorcell.png)");

//    doorId = new QLabel(this);
//    doorId->setStyleSheet("font-family:wenquanyi; font-size:30px; color: rgb(0, 255, 64)");
    payLabel = new QLabel(this);
    payLabel->resize(400,40);
    payLabel->move(0,200);
    payLabel->setAlignment(Qt::AlignCenter);
    payLabel->setStyleSheet("font-family:Droid Sans Fallback;font-weight:Bold;font-size:18px; color: rgb(255, 255, 0)");

}


void CTakeOpenCell::setCellLab(QString data)
{
    QString showCell = data.right(4);

    doorId->setText(showCell.left(2));
    doorCellId->setText(showCell.right(2));
    payLabel->hide();

    timer.start();
    this->exec();
}

void CTakeOpenCell::setCellLab(QString data, double payMoney, double balance)
{
    QString showCell = data.right(4);

    doorId->setText(showCell.left(2));
    doorCellId->setText(showCell.right(2));

    QString balanceStr = QString::number(balance,'f',2);
    payLabel->setText(QString("刷卡支付%1元，帐户余额%2元。").arg(payMoney).arg(balanceStr));
    payLabel->show();

    timer.start();
    this->exec();
}

void CTakeOpenCell::timerOver()
{
    emit displayOpenCell();
    this->close();
}
