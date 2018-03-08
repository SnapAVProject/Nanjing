#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QThread>
#include "singleton.h"
#include "lockdrv.h"
#include "db.h"
#include "exceptioncomm.h"
#include "servercomm.h"
#include "mainctrl.h"
#include <QObject>

#define EXCEPTION_PROC_INTERVAL  2*60

class CExceptionThrd : public QThread
{
    Q_OBJECT
public:

    void init()
    {
        start();
    }  

signals:
    void updateCell_CExceptionThrd();

private:
    CExceptionThrd(){}
    ~CExceptionThrd(){}

    CLockDrv lockoperation;  

    void run()
    {
        while(1)
        {
            sleep(EXCEPTION_PROC_INTERVAL);
            qDebug() << "--info--: exception thread proccess ...";
      
            if (SCMainCtrl::instance()->curStatus() == 1)
            {
               openExceptionProc();

               commExceptionProc();

               debtExceptionProc();
            }
        }

    }


    void openExceptionProc()
    {
        QList<CellDesc_t> celllist;
        SCDatBase::instance()->queryOpenException(celllist);
       // qDebug() << "--info--: open Exception, size " << celllist.size();
        QString terminal = SCDatBase::instance()->getTerminalId();

        qint8 bigBox = 0;
        qint8 middleBox = 0;
        qint8 smallBox = 0;
        qint8 vegetableBox = 0;

        for (int i = 0; i < celllist.size(); i++)
        {
            if(celllist.at(i).errType == "1") //修复未关闭
            {
                if (lockoperation.queryLock(celllist.at(i).cabinetId, celllist.at(i).cellId) == 0)
                {
                    QString scabinet = QString::number(celllist.at(i).cabinetId).sprintf("%02d",celllist.at(i).cabinetId);
                    QString scell    = QString::number(celllist.at(i).cellId).sprintf("%02d",celllist.at(i).cellId);
                  //  qDebug() << "--info--: " << scabinet << " ss" << scell;
                    repairCellErrorsResp repairRespone;
                    cellErrorInfo info;
                    info.scellId = terminal + scabinet + scell;
                    info.errType = "2";

                    SCExceptioncomm::instance()->repairCellErrors(info,&repairRespone);
                    if(repairRespone.success() == "true")
                    {
                        SCDatBase::instance()->deleteOpenException(celllist.at(i).index);
                        if(celllist.at(i).cellType == 1)
                        {
                            bigBox++;
                        }
                        else if(celllist.at(i).cellType == 2)
                        {
                            middleBox++;
                        }
                        else if(celllist.at(i).cellType == 3)
                        {
                            smallBox++;
                        }
                        else
                        {
                            vegetableBox++;
                        }
                    }

                }
            }
            else if(celllist.at(i).errType == "2")   //上报取件未关闭(取件完箱门默认是未关闭）
            {
                qint8 retQueryValue = lockoperation.queryLock(celllist.at(i).cabinetId, celllist.at(i).cellId);
                if(retQueryValue == 0)
                {
                     SCDatBase::instance()->deleteOpenException(celllist.at(i).index);

                     if(celllist.at(i).cellType == 1)
                     {
                         bigBox++;
                     }
                     else if(celllist.at(i).cellType == 2)
                     {
                         middleBox++;
                     }
                     else if(celllist.at(i).cellType == 3)
                     {
                         smallBox++;
                     }
                     else
                     {
                        vegetableBox++;
                     }
                }
                else if(retQueryValue == 1)
                {
                    //发送给服务器
                    QList<cellErrorInfo> boxExceptionList;
                    cellErrorInfo notCloseInfo;
                    QString takecabinet = QString::number(celllist.at(i).cabinetId).sprintf("%02d",celllist.at(i).cabinetId);
                    QString takecell    = QString::number(celllist.at(i).cellId).sprintf("%02d",celllist.at(i).cellId);

                    notCloseInfo.scabId = terminal + takecabinet;
                    notCloseInfo.scellId = terminal + takecabinet + takecell;
                    notCloseInfo.errType = "2";
                    boxExceptionList.append(notCloseInfo);

                    reportCellErrorsResp respone;
                    SCExceptioncomm::instance()->reportCellErrors(boxExceptionList,&respone);
                    if(respone.success() == "true")
                    {
                        //改变未打开箱门类型
                        SCDatBase::instance()->setOpenException(celllist.at(i).index);
                    }


                }
            }


        }

        if(bigBox || middleBox || smallBox || vegetableBox)
        {
            CellInfoDesc_t cellInfo;
            SCDatBase::instance()->getBoxCount(cellInfo);
            SCDatBase::instance()->setBoxCount(cellInfo.bigCell + bigBox,cellInfo.middleCell + middleBox,cellInfo.smallCell + smallBox,cellInfo.vegetableCell + vegetableBox);
            emit updateCell_CExceptionThrd();
        }
        //qDebug() << "openExceptionProc leave :";
    }


    void commExceptionProc()
    {
        QList<ServerDesc_t> serverlist;
        SCDatBase::instance()->queryServerException(serverlist);
        //qDebug() << "--info--: size " << serverlist.size();
        for (int i = 0; i < serverlist.size(); i++)
        {
             if (SCExceptioncomm::instance()->commReTrans(serverlist.at(i).url, serverlist.at(i).param))
             {
                 SCDatBase::instance()->deleteServerException(serverlist.at(i).index);
             }
        }

        //qDebug() << "commExceptionProc leave :";
    }

    void debtExceptionProc()
    {
        QList<DebtDesc_t>debtlist;
        updateAccountPaidResp respone;
        SCDatBase::instance()->queryDebtException(debtlist);
        //qDebug() << "--debt--info--: size " << debtlist.size();
        for(int i = 0;i < debtlist.size();i++)
        {
            QString debtStr = QString("%1").arg(debtlist.at(i).debt);
            SCServerComm::instance()->updateAccountPaid(debtlist.at(i).deliveryId,debtStr,&respone);
            if(respone.success() == "true")
            {
                SCDatBase::instance()->deleteDebtException(debtlist.at(i).index);
            }
        }
    }



    DECLARE_SINGLETON_CLASS(CExceptionThrd)
};

typedef Singleton<CExceptionThrd> SExceptionThrd;

#endif // EXCEPTION_H
