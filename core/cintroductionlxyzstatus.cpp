
#include "cintroductionlxyzstatus.h"
#include "mainctrl.h"

CIntroductionLXYZStatus::CIntroductionLXYZStatus(STATUS status):
    CStatus(status)
{
    introductionLxyz = new CIntroductionLXYZ();


    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}


void CIntroductionLXYZStatus::enter(QByteArray param)
{
    openTimer();
    introductionLxyz->show();
}

void CIntroductionLXYZStatus::leave(QByteArray param)
{
    stopTimer();
    introductionLxyz->hide();
}



void CIntroductionLXYZStatus::jumpPage()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
