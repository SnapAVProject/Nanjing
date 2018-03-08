#include "cadvertisement.h"
#include "ui_cadvertisement.h"
#include "driver.h"

CAdvertisement::CAdvertisement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAdvertisement)
{
    ui->setupUi(this);

    timer.setInterval(1*100);
    timer.setSingleShot(true);

    initPage();

    connect(&timer,SIGNAL(timeout()),this,SIGNAL(advPageClose()));
}

CAdvertisement::~CAdvertisement()
{
    delete ui;
}


void CAdvertisement::initPage()
{
    setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800,600);
    setObjectName("QWidget");
    setStyleSheet("#QWidget{border-image: url(./images/advertiser.png)}");
}


void CAdvertisement::advShowPage(QString deliveryId)
{
    //打开摄像头
    SCDriver::instance()->catchVideo(deliveryId);
    timer.start();

    this->show();
}
