#include "financestatus.h"
#include <QApplication>

CFinanceStatus::CFinanceStatus(STATUS status):
    CStatus(status)
{
    finance  = new financepage;

    connect(finance,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}


void CFinanceStatus::enter(QByteArray param)
{
    openTimer();

    finance->readInfo(QString(param));
    finance->show();

}

void CFinanceStatus::leave(QByteArray param)
{
    finance->hide();
    stopTimer();
}

void CFinanceStatus::keyInputProc(QByteArray param)
{
    clearTimer();
    QKeyEvent* evt = new QKeyEvent(QEvent::KeyPress, (int)param[0], Qt::NoModifier);
    QApplication::postEvent(QApplication::focusWidget() , evt);
}

void CFinanceStatus::resetStart()
{
    clearTimer();
}

void CFinanceStatus::jumpPage()
{
    finance->quitPage();
}
