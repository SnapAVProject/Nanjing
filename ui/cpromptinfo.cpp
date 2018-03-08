#include "cpromptinfo.h"
#include "ui_cpromptinfo.h"
#include <QTimer>
#include <QPainter>
#include <QBitmap>

CPromptInfo::CPromptInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CPromptInfo)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    timer->setInterval(6000);
    timer->setSingleShot(true);

    this->setWindowFlags(Qt::FramelessWindowHint);

    QPixmap pixmap(":/public/images/public/prompt.png");
    this->setMask(pixmap.mask());
    this->resize( pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);

   // this->setStyleSheet("QDialog{background-color:transparent;border-image: url(:/public/images/public/prompt.png)}");
    ui->logBtn->setFocusPolicy(Qt::NoFocus);
    ui->logBtn->setFlat(true);
    ui->logBtn->setStyleSheet("background-color:transparent;background-image: url(:/public/images/public/noIcBtn.png)");

    ui->closeBtn->setFocusPolicy(Qt::NoFocus);
    ui->closeBtn->setFlat(true);
    ui->closeBtn->setStyleSheet("background-color:transparent");

    connect(ui->logBtn,SIGNAL(clicked()),this,SIGNAL(logRequest()));
    connect(ui->logBtn,SIGNAL(clicked()),this,SLOT(closeICPromptPage()));
    connect(ui->closeBtn,SIGNAL(clicked()),this,SLOT(closeICPromptPage()));
    connect(timer,SIGNAL(timeout()),this,SLOT(close()));
}

CPromptInfo::~CPromptInfo()
{
    delete ui;
}


void CPromptInfo::showIcPromptPage()
{
    timer->start();
    this->exec();
}


void CPromptInfo::closeICPromptPage()
{
    timer->stop();
    this->close();
}
