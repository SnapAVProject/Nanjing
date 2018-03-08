#include "exphelpstatus.h"

CExpHelpStatus::CExpHelpStatus(STATUS status):
    CStatus(status)
{

    helppage = new exphelppage();

    connect(helppage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}


void CExpHelpStatus::enter(QByteArray param)
{
    openTimer();

    helppage->readInfo(param);
    helppage->show();
}


void CExpHelpStatus::leave(QByteArray param)
{
    stopTimer();
    helppage->hide();
}

void CExpHelpStatus::resetStart()
{
    clearTimer();
}

void CExpHelpStatus::jumpPage()
{
    helppage->exit_slots();
}


void CExpHelpStatus::keyInputProc(QByteArray param)
{

}

