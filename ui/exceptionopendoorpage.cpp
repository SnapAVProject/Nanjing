#include "exceptionopendoorpage.h"
#include "ui_exceptionopendoorpage.h"

#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include "mainctrl.h"
#include "db.h"
#include "servercomm.h"

exceptionopendoorpage::exceptionopendoorpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::exceptionopendoorpage)
{
    ui->setupUi(this);
    initPage();
}

exceptionopendoorpage::~exceptionopendoorpage()
{
    delete ui;
}


void exceptionopendoorpage::initPage()
{
    this->setObjectName("QWidget");
    this->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800,600);

    QLabel *prompt = new QLabel(this);
    prompt->resize(500,42);
    prompt->move(10,30);
    prompt->setStyleSheet("border-image: url(:/page/images/exception/word1.png)");

    QLabel *idCard = new QLabel(this);
    idCard->resize(75,35);
    idCard->move(210,176);
    idCard->setStyleSheet("border-image: url(:/page/images/exception/word2.png)");

    QStringList cabinetList;

    for(int i = 1; i < 8; i++)
    {
        cabinetList.append(QString::number(i).sprintf("%02d",i));
    }

    cabinetComBox = new QComboBox(this);
    cabinetComBox->resize(280,40);
    cabinetComBox->move(330,176);
    cabinetComBox->addItems(cabinetList);
    cabinetComBox->setStyleSheet("font-family:wenquanyi; font-size:24px");

    QLabel *cellLabel = new QLabel(this);
    cellLabel->resize(75,35);
    cellLabel->move(210,249);
    cellLabel->setStyleSheet("border-image: url(:/page/images/exception/word3.png)");

    QStringList cellList;
    for(int j = 1;j < 21;j++)
    {
        cellList.append(QString::number(j).sprintf("%02d",j));
    }
    cellComBox = new QComboBox(this);
    cellComBox->resize(280,40);
    cellComBox->move(330,249);
    cellComBox->addItems(cellList);
    cellComBox->setMaxVisibleItems(5);
    cellComBox->setStyleSheet("font-family:wenquanyi; font-size:24px");

    okBtn = new QPushButton(this);
    okBtn->resize(182,70);
    okBtn->move(170,494);
    okBtn->setFlat(true);
    okBtn->setFocusPolicy(Qt::NoFocus);
    okBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/exception/openBtn.png)");


    cancelBtn = new QPushButton(this);
    cancelBtn->resize(182,70);
    cancelBtn->move(453,494);
    cancelBtn->setFlat(true);
    cancelBtn->setFocusPolicy(Qt::NoFocus);
    cancelBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/exitBtn.png)");

    connect(okBtn,SIGNAL(clicked()),this,SLOT(okBtn_slots()));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtn_slots()));

}

void exceptionopendoorpage::okBtn_slots()
{
    emit resetTime();
    okBtn->setEnabled(false);
    cancelBtn->setEnabled(false);
    QString terminal = SCDatBase::instance()->getTerminalId();
    QString openCell = terminal + cabinetComBox->currentText() + cellComBox->currentText();

    if(filterCell(openCell))
    {
        myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("此箱门中已有物品，您无法打开！"));
    }
    else
    {
        openScellResp respone;
        SCServerComm::instance()->openScell(openCell,&respone);
        if(respone.success() == RET_FALSE && respone.errCode() == DEVICE_ERROR)
        {
            maintainPage();  //设备维护
        }
        else if(respone.success() == RET_FALSE && respone.errCode() == USE_CELLID)
        {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("此箱门中已有物品，您无法打开！"));
        }
        else if(respone.success() == RET_FALSE && respone.errCode() == NO_CELLID)
        {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("此箱门不存在，请确认箱门号！"));
        }
        else if(respone.success() == RET_TRUE)
        {
            qint8 cabinetId = cabinetComBox->currentText().toInt();
            qint8 cellId = cellComBox->currentText().toInt();
            if(lockoperation.openLock(cabinetId,cellId) != 1)
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("箱门打不开"));
            }
        }
        else
        {
             myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"));
        }
    }
    okBtn->setEnabled(true);
    cancelBtn->setEnabled(true);
}


bool exceptionopendoorpage::filterCell(QString openCell)
{

    QList<ServerDesc_t> serverlist;
    SCDatBase::instance()->queryServerException(serverlist);

    for (int i = 0; i < serverlist.size(); i++)
    {
        if(serverlist.at(i).errType == "1")
        {
            //exitDoor: 1,000XXXXXXXXX|2,000XXXXXXXXX|
            QString exitDoor = serverlist.at(i).cellId;
            QStringList exitDoorList = exitDoor.split("|");
            for(int j = 0; j < exitDoorList.size(); j++)
            {
                QString oneExitDoor = exitDoorList.at(j);
                QStringList oneExitDoorList = oneExitDoor.split(",");
                if(oneExitDoorList.last() == openCell)
                {
                    return true;
                }
            }
        }
    }

    return false;

}



void exceptionopendoorpage::maintainPage()
{
    cabinetComBox->setCurrentIndex(0);
    cellComBox->setCurrentIndex(0);
    SCMainCtrl::instance()->enterStatus(ST_DEVICE_REST,"");
}

void exceptionopendoorpage::cancelBtn_slots()
{
    cabinetComBox->setCurrentIndex(0);
    cellComBox->setCurrentIndex(0);
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}


