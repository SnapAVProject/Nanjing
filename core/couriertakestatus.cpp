#include "couriertakestatuse.h"
#include "mainctrl.h"

CourierTakePackageStatus::CourierTakePackageStatus(STATUS status) :
    CStatus(status)
{

    courierpick = new courierpickpage();

    connect(courierpick,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}



void CourierTakePackageStatus::enter(QByteArray param)
{
    openTimer();
    courierpick->readInfo(param);
    courierpick->show();
}

void CourierTakePackageStatus::leave(QByteArray param)
{
    stopTimer();
    courierpick->hide();
}

void CourierTakePackageStatus::resetStart()
{
    clearTimer();
}


void CourierTakePackageStatus::keyInputProc(QByteArray param)
{

}


void CourierTakePackageStatus::jumpPage()
{
    courierpick->quit_slots();
}
