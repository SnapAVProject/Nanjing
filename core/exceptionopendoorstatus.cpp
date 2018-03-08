#include "exceptionopendoorstatus.h"

CExceptionOpenDoorStatus::CExceptionOpenDoorStatus(STATUS status):
    CStatus(status)
{
    opendoorpage = new exceptionopendoorpage();

    connect(opendoorpage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}


void CExceptionOpenDoorStatus::enter(QByteArray param)
{
    openTimer();
    opendoorpage->show();
}

void CExceptionOpenDoorStatus::leave(QByteArray param)
{
    stopTimer();
    opendoorpage->hide();
}

void CExceptionOpenDoorStatus::resetStart()
{
    clearTimer();
}

void CExceptionOpenDoorStatus::jumpPage()
{
    opendoorpage->cancelBtn_slots();
}

void CExceptionOpenDoorStatus::keyInputProc(QByteArray param)
{

}
