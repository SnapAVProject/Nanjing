#include "vipbindstatus.h"
#include <QApplication>
#include <QKeyEvent>

CVipBindStatus::CVipBindStatus(STATUS status) :
    CStatus(status)
{
    bindpage = new vipbindpage();
    connect(bindpage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}


void CVipBindStatus::enter(QByteArray param)
{
    openTimer();
    bindpage->show();
}

void CVipBindStatus::leave(QByteArray param)
{
    stopTimer();
    bindpage->hide();
}

void CVipBindStatus::cardInputProc(QByteArray param)
{
    clearTimer();
    bindpage->setM1Card(QString(param));
}

void CVipBindStatus::keyInputProc(QByteArray param)
{
    clearTimer();
    QKeyEvent* evt = new QKeyEvent(QEvent::KeyPress, (int)param[0], Qt::NoModifier);
    QApplication::postEvent(QApplication::focusWidget() , evt);
}

void CVipBindStatus::resetStart()
{
    clearTimer();
}

void CVipBindStatus::jumpPage()
{
    bindpage->closePage();
}
