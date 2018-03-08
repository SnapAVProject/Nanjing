#include <QApplication>
#include "querybalancestatus.h"

CQueryBalanceStatus::CQueryBalanceStatus(STATUS status):
    CStatus(status)
{
    balancepage = new querybalancepage;

    connect(balancepage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
    connect(this,SIGNAL(cardcoming(QString)),balancepage,SLOT(readCardInfo(QString)));
}


void CQueryBalanceStatus::enter(QByteArray param)
{
     openTimer();
     balancepage->show();
}


void CQueryBalanceStatus::leave(QByteArray param)
{
     stopTimer();
     balancepage->hide();
}

void CQueryBalanceStatus::cardInputProc(QByteArray param)
{
    clearTimer();
    emit cardcoming(QString(param));
}


void CQueryBalanceStatus::keyInputProc(QByteArray param)
{
    clearTimer();
    QKeyEvent* evt = new QKeyEvent(QEvent::KeyPress, (int)param[0], Qt::NoModifier);
    QApplication::postEvent(QApplication::focusWidget() , evt);
}

void CQueryBalanceStatus::resetStart()
{
    clearTimer();
}

void CQueryBalanceStatus::jumpPage()
{

    balancepage->cancelBtn_slots();
}
