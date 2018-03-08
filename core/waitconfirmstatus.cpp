#include "waitconfirmstatus.h"
#include "mainctrl.h"
#include "servercomm.h"
#include "configure/db.h"
#include <QKeyEvent>
#include <QApplication>

CWaitconfirmStatus::CWaitconfirmStatus(STATUS status):
        CStatus(status)
{
        waitconfirm = new waitconfirmpage;

        connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}


void CWaitconfirmStatus::enter(QByteArray param)
{
    openTimer();
    QString m1CardId;
    QDataStream in(&param,QIODevice::ReadOnly);
    in >> m1CardId;

    waitconfirm->readwaitconfirminfo(param);
    waitconfirm->show();


}

void CWaitconfirmStatus::leave(QByteArray param)
{
    stopTimer();
 //   qDebug() << param;
    waitconfirm->hide();
    //delete waitconfirm;
   // waitconfirm = NULL;
}

void CWaitconfirmStatus::jumpPage()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

void CWaitconfirmStatus::keyInputProc(QByteArray param)
{
    QKeyEvent* evt = new QKeyEvent(QEvent::KeyPress, (int)param[0], Qt::NoModifier);
    QApplication::postEvent(QApplication::focusWidget() , evt);
}
