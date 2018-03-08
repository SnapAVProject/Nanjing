#include "setportstatus.h"
#include <QApplication>
#include <QKeyEvent>
#include <QDebug>
#include "mainctrl.h"

CSetportstatus::CSetportstatus(STATUS status):
    CStatus(status)
{
    portpage = new setportpage();

    connect(portpage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void CSetportstatus::enter(QByteArray param)
{
    openTimer();
    portpage->readinfo();
    portpage->show();
 // qDebug() << "enter set port status";

}

void CSetportstatus::leave(QByteArray param)
{
    stopTimer();
 //   qDebug() << "lenve set port status";
    portpage->hide();
   // delete portpage;
   // portpage = NULL;
    //portpage->hide();
}


void CSetportstatus::keyInputProc(QByteArray param)
{
    clearTimer();
    QKeyEvent* evt = new QKeyEvent(QEvent::KeyPress, (int)param[0], Qt::NoModifier);
    QApplication::postEvent(QApplication::focusWidget() , evt);
}

void CSetportstatus::coinInputProc(QByteArray param)
{
    clearTimer();
    portpage->recordCoin();
}

void CSetportstatus::upsInputProc(QByteArray param)
{
    clearTimer();
    portpage->upsTest();
}


void CSetportstatus::resetStart()
{
    clearTimer();
}

void CSetportstatus::jumpPage()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
