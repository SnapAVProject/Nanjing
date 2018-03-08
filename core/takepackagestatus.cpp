#include "takepackagestatus.h"
#include <QApplication>

CTakePageStatus::CTakePageStatus(STATUS status):
     CStatus(status)
{

   getpackage = new takepackagepage;

   connect(getpackage,SIGNAL(resetTime()),this,SLOT(resetStart()));
 //  connect(this, SIGNAL(coincoming()), getpackage,SLOT(payCoin_slot()));
   connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
 //  connect(this,SIGNAL(cardcoming(QString)),getpackage,SLOT(payCard_slot(QString)));

}

void CTakePageStatus::enter(QByteArray param)
{
 //   qDebug() << param <<"CTake is coming";

    openTimer();

    getpackage->initPage();
    getpackage->show();

}


void CTakePageStatus::leave(QByteArray param)
{
    stopTimer();
 //   qDebug() << param << "CTake is leave";
    if(param.isEmpty())
    {
        getpackage->hide();
    }
    else
    {
        QTimer::singleShot(1000, this, SLOT(leaveSlots()));
    }
   // getpackage->hide();
   // delete getpackage;
   // getpackage = NULL;
}


void CTakePageStatus::leaveSlots()
{
    getpackage->hide();
}


void CTakePageStatus::coinInputProc(QByteArray param)
{
    clearTimer();
    //qDebug() << param;
    getpackage->payCoin();
   // emit coincoming();
}

void CTakePageStatus::cardInputProc(QByteArray param)
{
    clearTimer();
    getpackage->payCard(QString(param));
   // emit cardcoming(QString(param));
}

void CTakePageStatus::keyInputProc(QByteArray param)
{
    clearTimer();
    QKeyEvent* evt = new QKeyEvent(QEvent::KeyPress, (int)param[0], Qt::NoModifier);
    QApplication::postEvent(QApplication::focusWidget() , evt);
}

void CTakePageStatus::resetStart()
{
    clearTimer();
}

void CTakePageStatus::jumpPage()
{
    getpackage->quitTakePack_slots();
}

