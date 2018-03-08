#include "exceptionstatus.h"
#include "servercomm.h"
#include "db.h"
#include "mainctrl.h"

CExceptionStatus::CExceptionStatus(STATUS status):
    CStatus(status)
{
    abnormalpage = new exceptionpage();
    myMessageBox = new CMessageBox(3*1000);

    connect(abnormalpage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));

}


void CExceptionStatus::enter(QByteArray param)
{
    openTimer();
    abnormalpage->show();
}

void CExceptionStatus::leave(QByteArray param)
{
    stopTimer();
    abnormalpage->hide();
}

void CExceptionStatus::resetStart()
{
    clearTimer();
}

void CExceptionStatus::cardInputProc(QByteArray param)
{
    if(abnormalpage->getIsWorking() == 0)
    {
        myMessageBox->myPrompt(QMessageBox::Warning,tr("提示"),tr("请选择您需要处理的类型。"));
    }
    else
    {
        abnormalpage->recvCardInfo(QString(param));
    }

}


void CExceptionStatus::jumpPage()
{
    abnormalpage->exit_slots();
}
