#include "cardcheckstatus.h"

CCardcheckStatus::CCardcheckStatus(STATUS status):
    CStatus(status)
{

    cardpage = new checkcardpage();
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}



void CCardcheckStatus::enter(QByteArray param)
{
    openTimer();
    cardpage->show();
}


void CCardcheckStatus::leave(QByteArray param)
{
    stopTimer();
    cardpage->hide();
}


void CCardcheckStatus::cardInputProc(QByteArray param)
{
    clearTimer();
    cardpage->readCardInfo(QString(param));
}

void CCardcheckStatus::jumpPage()
{
    cardpage->exitBtn_slots();
}
