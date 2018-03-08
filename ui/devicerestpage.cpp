#include "devicerestpage.h"
#include "ui_devicerestpage.h"

#include "db.h"
#include "servercomm.h"
#include "mainctrl.h"
#include <QLabel>
#include <QTimer>
#include <QBoxLayout>

#define TIME_OUT (15*60*1000)

devicerestpage::devicerestpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::devicerestpage)
{
    ui->setupUi(this);
    initPage();
}

devicerestpage::~devicerestpage()
{
    delete ui;
}


void devicerestpage::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800,600);
    this->setStyleSheet("QDialog{background-image: url(:/page/images/device/device.png)}");

    timer = new QTimer(this);

    connect(timer,SIGNAL(timeout()),this,SLOT(timeOver()));
}


void devicerestpage::startQueryDevice()
{
    timer->start(TIME_OUT);
}


void devicerestpage::timeOver()
{
    getSystemInfoResp respone;

    SCServerComm::instance()->getSystemInfo(&respone);

    if(respone.systemMaintenance() == "no")
    {
        timer->stop();
        SCMainCtrl::instance()->enterStatus(ST_HOME,"");
    }
}
