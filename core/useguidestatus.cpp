#include "useguidestatus.h"
#include "mainctrl.h"

CUseguideStatus::CUseguideStatus(STATUS status):
    CStatus(status)
{
    useguide = new useguidepage;

    connect(useguide,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}


void CUseguideStatus::enter(QByteArray param)
{
    openTimer();
    useguide->readInfo();
    useguide->show();
}

void CUseguideStatus::leave(QByteArray param)
{
    stopTimer();
    useguide->hide();
}


void CUseguideStatus::resetStart()
{
    clearTimer();
}

void CUseguideStatus::jumpPage()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
