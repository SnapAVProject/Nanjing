#include "savepackagewaitpage.h"
#include "mainctrl.h"
#include "driver.h"
#include "db.h"
#include "lockdrv.h"
#include <QDataStream>
#include <QDebug>

//***************************************************************************
/**file		savepackagewaitpage.cpp
* brief		存件
* author	hjzhang@163.com	2013/9/26
* version	1.3
*/
//***************************************************************************

savepackagewaitpage::savepackagewaitpage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    isOpenBox = false;
    initWaitpage();
    connect(&isOverTime,SIGNAL(timeout()),this,SLOT(overTime()));
    connect(&queryLock,SIGNAL(timeout()),this,SLOT(queryLockStatus()));
}


void savepackagewaitpage::initWaitpage()
{
    this->resize(800,600);
    this->setObjectName("QWidget");
    this->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");


    myMessageBox = new CMessageBox(120*1000);
    myBox = new CMessageBox(5*1000);

    waitSaveTime = 120;
    QLabel *label = new QLabel(this);
    label->resize(689,82);
    label->move(50,30);
    label->setStyleSheet("border-image: url(:/page/images/saveWait/word1.png)");

    timeLabel = new QLabel(this);
    timeLabel->move(380,200);
    timeLabel->setStyleSheet("font-family:wenquanyi; font-weight:bold; font-size:36px; color: rgb(255, 0, 0)");
    timeLabel->setText(QString("%1").arg(waitSaveTime));

    QLabel *door = new QLabel(this);
    door->resize(86,44);
    door->move(270,250);
    door->setStyleSheet("background-image: url(:/page/images/saveWait/door.png)");

    doorId = new QLabel(this);
    doorId->setAlignment(Qt::AlignCenter);
    doorId->resize(40,40);
    doorId->move(360,253);
    doorId->setStyleSheet("font-family:wenquanyi; font-size:30px; color: rgb(0, 255, 64)");

    QLabel *doorCabinet = new QLabel(this);
    doorCabinet->resize(37,40);
    doorCabinet->move(410,253);
    doorCabinet->setStyleSheet("background-image: url(:/page/images/saveWait/doorcabinet.png)");

    doorCellId = new QLabel(this);
    doorCellId->move(450,253);
    doorCellId->setStyleSheet("font-family:wenquanyi; font-size:30px; color: rgb(0, 255, 64)");

    QLabel *doorCell = new QLabel(this);
    doorCell->resize(37,40);
    doorCell->move(500,253);
    doorCell->setStyleSheet("background-image: url(:/page/images/saveWait/doorcell.png)");


    okBtn = new QPushButton(this);
    okBtn->setEnabled(false);
    okBtn->resize(182,70);
    okBtn->move(170,494);
    okBtn->setFlat(true);
    okBtn->setFocusPolicy(Qt::NoFocus);
    okBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/saveWait/okBtn_bg.png)");

    cancelBtn = new QPushButton(this);
    cancelBtn->setEnabled(false);
    cancelBtn->resize(182,70);
    cancelBtn->move(453,494);
    cancelBtn->setFlat(true);
    cancelBtn->setFocusPolicy(Qt::NoFocus);
    cancelBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/saveWait/cancelBtn_bg.png)");


    connect(myMessageBox,SIGNAL(message_ok()),this,SLOT(openLockSlot()));
    connect(myMessageBox,SIGNAL(message_cancel()),this,SLOT(changeBoxSlot()));
    connect(okBtn,SIGNAL(clicked()),this,SLOT(okBtnSlot()));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtnSlot()));

}

/****************************************************************
**
*	函数名称：	okBtnSlot
*	函数功能：	确定按钮
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void savepackagewaitpage::okBtnSlot()
{
    isOverTime.stop();
    QString isShowList = "ADD_DATA";
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out << isShowList << notOpenBoxList << notCheckBoxList << M1Id << daemonInfo.packageId << daemonInfo.mobilePhone  << daemonInfo.cellType;
    out << daemonInfo.cellId << daemonInfo.isConfirm << daemonInfo.isCheck;

    SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_LIST,data);

    clearInfo();

}

/****************************************************************
**
*	函数名称：	cancelBtnSlot
*	函数功能：	取消按钮
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void savepackagewaitpage::cancelBtnSlot()
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out << M1Id << boxType;

    isOverTime.stop();

    SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_MENU,data);

    clearInfo();

}


/****************************************************************
**
*	函数名称：	clearInfo
*	函数功能：	清空数据
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void savepackagewaitpage::clearInfo()
{
    M1Id.clear();
    boxType.clear();

    daemonInfo.packageId.clear();
    daemonInfo.mobilePhone.clear();
    daemonInfo.cellType.clear();
    daemonInfo.cellId.clear();
    daemonInfo.isConfirm.clear();
    daemonInfo.isCheck.clear();


}

void savepackagewaitpage::setBtnEnable(bool status)
{
    this->okBtn->setEnabled(status);
    this->cancelBtn->setEnabled(status);
}


/****************************************************************
**
*	函数名称：	overTime
*	函数功能：	当超时时，锁箱
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void savepackagewaitpage::overTime()
{

    waitSaveTime--;
    if(waitSaveTime <= 0)
    {
        QString isShowList;
        QByteArray data;
        qint8 cellType;
        QMap<QString,qint8> notCloseCell;
        QDataStream out(&data,QIODevice::WriteOnly);
        //自动退出、锁箱、发送信息给后台
        if(isOpenBox)
        {
            isOpenBox = false;
            this->daemonInfo.isConfirm = "no";
            isShowList = "ADD_DATA_NO_Wait";  //ADD_DATA_NO_Wait 要添加下页面不要等待 ADD_DATA:需要添加 NO_ADD_DATA:不需要添加信息直接发送 NO_Wait:再次打不开，直接跳转

            out << isShowList << notOpenBoxList << notCheckBoxList << M1Id << daemonInfo.packageId << daemonInfo.mobilePhone  << daemonInfo.cellType;
            out << daemonInfo.cellId << daemonInfo.isConfirm << daemonInfo.isCheck;
        }
        else
        {
            isShowList = "NO_ADD_DATA";  //未关闭的箱门
            if(boxType == "BG")
            {
                cellType = 1;
            }
            else if(boxType == "MD")
            {
                cellType = 2;
            }
            else if(boxType == "SM")
            {
                cellType = 3;
            }
            else
            {
                cellType = 4;
            }

            notCloseCell.insert(daemonInfo.cellId,cellType);

            out << isShowList << notOpenBoxList << notCheckBoxList << notCloseCell << M1Id;
        }

        setBtnEnable(false);
        isOverTime.stop();

        queryLock.stop();
        SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_LIST,data);
        //发送消息给后台
        clearInfo();

    }
    else
    {
        this->timeLabel->setText(QString("%1").arg(waitSaveTime));
    }

}

/****************************************************************
**
*	函数名称：	queryLockStatus
*	函数功能：	查询锁的状态
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void savepackagewaitpage::queryLockStatus()
{
    bool ok;
    qint8 status;

    QString boxNumber = daemonInfo.cellId.right(4);
    qint8 cabinetId = boxNumber.left(2).toInt(&ok,10);
    qint8 cellId    = boxNumber.right(2).toInt(&ok,10);


    status = lockoperation.queryLock(cabinetId,cellId);
    if(status == 0)
    {
        queryLock.stop();
        isOpenBox = true;

        if(SCDriver::instance()->readCheckGoods())
        {
            if(0 == lockoperation.queryGoods(cabinetId,cellId))
            {
                daemonInfo.isCheck = "1";  //有物品
                okBtnSlot();
            }
            else
            {
                daemonInfo.isCheck = "2";  //无物品
                myMessageBox->setCheckTimer(waitSaveTime * 1000);
                myMessageBox->myPrompt(QMessageBox::Warning,tr("提示确认"),QString("系统未检测到%1箱%2柜包裹，是否打开重新放置？").arg(cabinetId).arg(cellId));

            }
        }
        else
        {
            this->okBtn->setEnabled(true);
            this->cancelBtn->setEnabled(true);
            okBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/saveWait/okBtn.png)");
            cancelBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/saveWait/cancelBtn.png)");
        }

    }



}

void savepackagewaitpage::changeBoxSlot()
{
    this->cancelBtn->setEnabled(true);
    cancelBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/saveWait/cancelBtn.png)");
}



/****************************************************************
**
*	函数名称：	openLockSlot
*	函数功能：	重新打开锁的状态
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void savepackagewaitpage::openLockSlot()
{
    QString boxNumber = daemonInfo.cellId.right(4);
    qint8 cabinetId = boxNumber.left(2).toInt();
    qint8 cellId    = boxNumber.right(2).toInt();
    waitSaveTime = 120;

    this->cancelBtn->setEnabled(false);
    cancelBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/saveWait/cancelBtn_bg.png)");

    if(lockoperation.openLock(cabinetId,cellId) == 1)
    {
        queryLock.start(3000);
        isOpenBox = false;
    }
    else
    {
        QString isShowList = "NO_Wait";
        QString notCheckCell = daemonInfo.cellId;
        qint8 cellType = daemonInfo.cellType.toInt();
        notOpenBoxList.insert(notCheckCell,cellType);

        QByteArray data;
        QDataStream out(&data,QIODevice::WriteOnly);

        out << isShowList << notOpenBoxList << notCheckBoxList << M1Id;

        myBox->myPrompt(QMessageBox::Warning,tr("提示"),tr("箱门打不开，正保存存件信息。"));

        isOverTime.stop();

        SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_LIST,data);
        //发送消息给后台

        clearInfo();
    }
}

/****************************************************************
**
*	函数名称：	readInfo
*	函数功能：	读数据
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void savepackagewaitpage::readInfo(QByteArray param)
{
    waitSaveTime = 120;
    isOpenBox = false;

    timeLabel->setText(QString("%1").arg(waitSaveTime));
    QDataStream in(&param,QIODevice::ReadOnly);
    in >> M1Id >> daemonInfo.packageId >> daemonInfo.mobilePhone  >> daemonInfo.cellType;
    in >> daemonInfo.cellId >> daemonInfo.isConfirm >> daemonInfo.isCheck >> boxType >> notOpenBoxList >> notCheckBoxList;

//    qDebug() << "M1Id:" << M1Id << "boxType:" << boxType << "notOpenList:" << notOpenBoxList;
    okBtn->setEnabled(false);
    cancelBtn->setEnabled(false);
    okBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/saveWait/okBtn_bg.png)");
    cancelBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/saveWait/cancelBtn_bg.png)");


    QString showCell = daemonInfo.cellId.right(4);
    doorId->setText(showCell.left(2));
    doorCellId->setText(showCell.right(2));

    isOverTime.start(1000);
    queryLock.start(3000);

}
