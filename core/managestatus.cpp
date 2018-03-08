#include "managestatus.h"
#include <QKeyEvent>
#include <QApplication>
#include "mainctrl.h"

CManageStatus::CManageStatus(STATUS status):
        CStatus(status)
{
    manage  = new managepage;

    connect(manage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void CManageStatus::enter(QByteArray param)
{
    openTimer();
    //qDebug() << param;

    manage->readinfo();
    manage->show();


}


void CManageStatus::leave(QByteArray param)
{
    stopTimer();
    //qDebug() << param;
    manage->hide();
}


void CManageStatus::keyInputProc(QByteArray param)
{
    clearTimer();
    QKeyEvent* evt = new QKeyEvent(QEvent::KeyPress, (int)param[0], Qt::NoModifier);
    QApplication::postEvent(QApplication::focusWidget() , evt);
}

void CManageStatus::resetStart()
{
    clearTimer();
}

void CManageStatus::jumpPage()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
