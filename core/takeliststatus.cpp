#include "takeliststatus.h"
#include <QApplication>

CTakeListStatus::CTakeListStatus(STATUS status):
    CStatus(status)
{
    pickpage = new cardpickpage();

    connect(pickpage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}


void CTakeListStatus::enter(QByteArray param)
{
    openTimer();
    pickpage->readInfo(param);
    pickpage->show();
}

void CTakeListStatus::leave(QByteArray param)
{
    stopTimer();
    pickpage->hide();
}


void CTakeListStatus::keyInputProc(QByteArray param)
{
//    clearTimer();
//    QKeyEvent* evt = new QKeyEvent(QEvent::KeyPress, (int)param[0], Qt::NoModifier);
//    QApplication::postEvent(QApplication::focusWidget() , evt);
}

void CTakeListStatus::resetStart()
{
    clearTimer();
}


void CTakeListStatus::jumpPage()
{
    pickpage->quitPage();
}
