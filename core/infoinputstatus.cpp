
#include "infoinputstatus.h"
#include <QApplication>

CInfoinputStatus::CInfoinputStatus(STATUS status):
        CStatus(status)
{
     infoinput = new infoinputpage;

     connect(infoinput,SIGNAL(resetTime()),this,SLOT(resetStart()));
     connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
     connect(this,SIGNAL(cardcoming(QString)),infoinput,SLOT(readCardInfo(QString)));

}



void CInfoinputStatus::enter(QByteArray param)
{
    openTimer();
    infoinput->readInfo(param);
    infoinput->show();
}

void CInfoinputStatus::leave(QByteArray param)
{
    stopTimer();
    infoinput->hide();

}

void CInfoinputStatus::resetStart()
{
    clearTimer();
}

void CInfoinputStatus::jumpPage()
{
    infoinput->cancelBtn_slots();
}



void CInfoinputStatus::cardInputProc(QByteArray param)
{

    emit cardcoming(QString(param));
}


void CInfoinputStatus::keyInputProc(QByteArray param)
{
    clearTimer();
    QKeyEvent* evt = new QKeyEvent(QEvent::KeyPress, (int)param[0], Qt::NoModifier);
    QApplication::postEvent(QApplication::focusWidget() , evt);
}
