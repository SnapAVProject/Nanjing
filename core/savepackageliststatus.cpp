#include "savepackageliststatus.h"
#include "mainctrl.h"
#include <QObject>

CSavepackagelistStatus::CSavepackagelistStatus(STATUS status) :
     CStatus(status)
{
    savelistpage = new savepackagelistpage();

    connect(&mTimer, SIGNAL(timeout()), this, SLOT(timeOutComing()));
    connect(this->savelistpage, SIGNAL(timeStop()), this, SLOT(timeStop()));
}


void CSavepackagelistStatus::enter(QByteArray param)
{
    mTimer.setSingleShot(true);
    mTimer.setInterval(120*1000);

    QString isShowList;
    QMap<QString,qint8> notOpenBoxList;
    QMap<QString,qint8> notCheckBoxList;
    QMap<QString,qint8> notCloseBoxOrM1;
    QDataStream in(&param,QIODevice::ReadOnly);
    in >> isShowList >> notOpenBoxList >> notCheckBoxList >> notCloseBoxOrM1;

    savelistpage->setNotOpenBox(notOpenBoxList);
    savelistpage->setNotCheckBox(notCheckBoxList);

    if(isShowList == "ADD_DATA")//||  savelistpage->daemonListSize() //不需要增加信息直接发送
    {
        savelistpage->readInfo(param);
        savelistpage->show();
  //      qDebug() << "isShowList:" << "ADD_NO_SHOW";

    }
    else if(isShowList == "ADD_DATA_NO_Wait")
    {
        savelistpage->readInfo(param);
        mTimer.setInterval(60);
        savelistpage->hide();
    }
    else if(isShowList == "NO_ADD_DATA")
    {
  //      qDebug() << "isShowList:" << "NO_ADD_SHOW";
        savelistpage->setNotCloseBox(notCloseBoxOrM1);
        mTimer.setInterval(10);
        savelistpage->hide();
    }
    else
    {
        mTimer.setInterval(10);
        savelistpage->hide();
    }

    mTimer.start();
}

void CSavepackagelistStatus::leave(QByteArray param)
{
    mTimer.stop();
  //  savelistpage->hide();
    QTimer::singleShot(1000, this, SLOT(leaveSlots()));
 //   qDebug() << "leave save ";
}

void CSavepackagelistStatus::leaveSlots()
{
    savelistpage->hide();
    //qDebug() <<  QTime::currentTime().toString();

}


void CSavepackagelistStatus::timeOutComing()
{
  //  qDebug() << "list time out";
    savelistpage->okBtnSlots();
}

void CSavepackagelistStatus::timeStop()
{
    mTimer.stop();
}
