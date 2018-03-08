#include "noiclogstatus.h"
#include <QTime>
#include <QApplication>
#include "mainctrl.h"

CNoiclogStatus::CNoiclogStatus(STATUS status):
        CStatus(status)
{
    noicsave = new noiclogpage;

    connect(noicsave,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}
void CNoiclogStatus::enter(QByteArray param)
{
   // qDebug() << "enter CNoiclogStatus" << param;
    openTimer();

    noicsave->initPage(param);
    noicsave->show();  

}


void CNoiclogStatus::leave(QByteArray param)
{
    stopTimer();
 //   qDebug() << param << " leave CNoiclogStatus";
    noicsave->hide();
}


void CNoiclogStatus::keyInputProc(QByteArray param)
{
    clearTimer();
    QKeyEvent* evt = new QKeyEvent(QEvent::KeyPress, (int)param[0], Qt::NoModifier);
    QApplication::postEvent(QApplication::focusWidget() , evt);
}

void CNoiclogStatus::resetStart()
{
    clearTimer();
}

void CNoiclogStatus::jumpPage()
{
 //   qDebug() << "CNoiclogStatus end timer";
    noicsave->cancelLogSavePage_slot();
}
