#include "savepackagestatus.h"
#include "driver.h"
#include <QApplication>

CSavepackageStatus::CSavepackageStatus(STATUS status):
        CStatus(status)
{
     savepackage = new savepackagepage;

     connect(&mTimer,SIGNAL(timeout()),this,SLOT(timeOutComing()));
     connect(savepackage,SIGNAL(resetTime()),this,SLOT(resetStart()));
     connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void CSavepackageStatus::enter(QByteArray param)
{
    openTimer();
    data = param;
    mTimer.setSingleShot(true);
    mTimer.setInterval(100);

    savepackage->setBtnEable(false);
    savepackage->readdisplayinfo(param);
    savepackage->show();
    mTimer.start();


}

void CSavepackageStatus::leave(QByteArray param)
{
    stopTimer();

    SCDriver::instance()->handleCodeScanQuit();
   // savepackage->hide();
    QTimer::singleShot(1000, this, SLOT(leaveSlots()));
    //qDebug() << "leave save ";
   // savepackage->closeEventFilter();
    //delete savepackage;
   // savepackage = NULL;
}


void CSavepackageStatus::leaveSlots()
{
    savepackage->hide();
}

void CSavepackageStatus::keyInputProc(QByteArray param)
{
    clearTimer();
    QKeyEvent* evt = new QKeyEvent(QEvent::KeyPress, (int)param[0], Qt::NoModifier);
    QApplication::postEvent(QApplication::focusWidget() , evt);
}


void CSavepackageStatus::timeOutComing()
{
    QString isLong;
    QString M1Id;
    QDataStream in(&data,QIODevice::ReadOnly);
    in >> M1Id >> isLong;

    savepackage->setBtnEable(true);
    if(isLong == "KD" || isLong == "NO" || isLong == "SC")
    {
        savepackage->loadcodeScan();
    }

}


void CSavepackageStatus::resetStart()
{
    clearTimer();
}

void CSavepackageStatus::jumpPage()
{
     savepackage->cancelslots();
}




