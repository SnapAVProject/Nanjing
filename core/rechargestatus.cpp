#include "rechargestatus.h"
#include "mainctrl.h"
#include <QApplication>
#include <QKeyEvent>

CRechargeStatus::CRechargeStatus(STATUS status):
    CStatus(status)
{
    rechar = new rechargepage;
    //SCMainCtrl::instance()->addPageStatus(this->status(),rechar);

    connect(rechar,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
    connect(this,SIGNAL(cardcoming(QString)),rechar,SLOT(readCardInfo(QString)));
}


void CRechargeStatus::enter(QByteArray param)
{
    openTimer();
    rechar->initPageInfo();
    rechar->show();
}

void CRechargeStatus::leave(QByteArray param)
{
    stopTimer();
    rechar->hide();
}

void CRechargeStatus::keyInputProc(QByteArray param)
{
    clearTimer();
    QKeyEvent* evt = new QKeyEvent(QEvent::KeyPress, (int)param[0], Qt::NoModifier);
    QApplication::postEvent(QApplication::focusWidget() , evt);
}

void CRechargeStatus::cardInputProc(QByteArray param)
{
    emit cardcoming(QString(param));
}



void CRechargeStatus::resetStart()
{
    clearTimer();
}

void CRechargeStatus::jumpPage()
{
    rechar->cancelbtn_slots();
}
