#include "mymessage.h"
#include "ui_mymessage.h"
#include <QDesktopWidget>
#include <QIcon>
#include <QDebug>
#include <QBitmap>

CMessageBox::CMessageBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mymessage)
{
    ui->setupUi(this);

    initPage();
    timer.setInterval(50*1000);
    timer.setSingleShot(true);
    connect(&timer,SIGNAL(timeout()),this,SLOT(stopTimer()));
    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(stopTimer()));
    connect(ui->okBtn,SIGNAL(clicked()),this,SIGNAL(message_ok()));
    connect(ui->cancelBtn,SIGNAL(clicked()),this,SLOT(stopTimer()));
    connect(ui->cancelBtn,SIGNAL(clicked()),this,SIGNAL(message_cancel()));
}


CMessageBox::CMessageBox(long time,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mymessage)
{
    ui->setupUi(this);

    initPage();
    timer.setInterval(time);
    timer.setSingleShot(true);
    connect(&timer,SIGNAL(timeout()),this,SLOT(stopTimer()));
    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(stopTimer()));
    connect(ui->okBtn,SIGNAL(clicked()),this,SIGNAL(message_ok()));
    connect(ui->cancelBtn,SIGNAL(clicked()),this,SLOT(stopTimer()));
    connect(ui->cancelBtn,SIGNAL(clicked()),this,SIGNAL(message_cancel()));
}

CMessageBox::~CMessageBox()
{
    delete ui;
}

void CMessageBox::stopTimer()
{
    emit message_close();
    timer.stop();
    this->close();
}

void CMessageBox::setCheckTimer(long time)
{
    timer.setInterval(time);
}

void CMessageBox::initPage()
{
    QPixmap pixmap(":/page/images/prompt/prompt.png");
    this->setMask(pixmap.mask());
    this->resize( pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->move((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);


    ui->promptLabel->setFixedSize(165,43);
    ui->promptLabel->setStyleSheet("border-image:url(:/page/images/prompt/word.png)");

    ui->okBtn->setFixedSize(117,45);
    ui->okBtn->setFlat(true);
    ui->okBtn->setFocusPolicy(Qt::NoFocus);
    ui->okBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/okBtn_p.png)");

    ui->cancelBtn->setFixedSize(117,45);
    ui->cancelBtn->setFlat(true);
    ui->cancelBtn->setFocusPolicy(Qt::NoFocus);
    ui->cancelBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/cancelBtn_p.png)");

    ui->wordLabel->setWordWrap(true);
    ui->wordLabel->setStyleSheet("font-family:wenquanyi;font-size:24px;color:rgb(255,255,255)");
    ui->wordLabel->setAlignment(Qt::AlignCenter);
}


void CMessageBox::myPrompt(QMessageBox::Icon icon,const QString & title, const QString & text,Qt::WindowModality model)
{
    timer.start();

    ui->wordLabel->setText(text);
    switch(icon)
    {
    case QMessageBox::NoIcon:

        break;
    case QMessageBox::Information:
        ui->imageLabel->setStyleSheet("background-image:url(:/images/success.png)");
        break;
    case QMessageBox::Warning:
        ui->imageLabel->setStyleSheet("background-image:url(:/images/warning.png)");
        break;
    case QMessageBox::Critical:
        ui->imageLabel->setStyleSheet("background-image:url(:/images/error.png)");
        break;
    case QMessageBox::Question:
        ui->imageLabel->setStyleSheet("background-image:url(:/images/prompt.png)");
        break;
    default:
        break;
    }


    if(title == tr("提示"))
    {
        ui->cancelBtn->hide();
    }
    else
    {
        ui->cancelBtn->show();
    }
    if(model == Qt::NonModal)
    {
        this->show();
    }
    else
    {
        this->exec();
    }
}



