#include "pricecheckstatus.h"
#include "mainctrl.h"
#include "pricecheck.h"
#include "mymessage.h"

#include <QDate>

pricecheckstatus::pricecheckstatus(STATUS status):
        CStatus(status)
{
    pricecheckUi = new pricecheck;
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
    fristDay = 0;
    myMessageBox = new CMessageBox(3*1000);
}
void pricecheckstatus::enter(QByteArray param)
{
    openTimer();
    QDate date;
    int secondDay = date.currentDate().day();
    if(fristDay != secondDay)
    {
        fristDay = secondDay;
        pricecheckUi->readInfo();
    }
    pricecheckUi->show();
}

void pricecheckstatus::leave(QByteArray param)
{
    stopTimer();
    pricecheckUi->hide();
}

void pricecheckstatus::jumpPage()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
