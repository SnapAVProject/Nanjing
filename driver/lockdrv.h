#ifndef LOCKDRV_H
#define LOCKDRV_H

#include <QObject>
#include "clockthread.h"
#include <QEventLoop>
#include <QTimer>
#include <QTime>

class CLockDrv : public QObject
{
    Q_OBJECT

public:

    CLockDrv()
    {
    }

    bool openLock(qint8 cabintId, qint8 cellId)
    {
        mstatus = -1;
        SCLockThread::instance()->openLock(cabintId, cellId, this);     
       
        QEventLoop loop;
        connect(this, SIGNAL(signalProtoclResp()), &loop, SLOT(quit()),Qt::DirectConnection);
        loop.exec();
 
        if (1 == mstatus)
            return true;
        else
            return false;

     }

    qint8 queryLock(qint8 cabintId, qint8 cellId)
    {
        mstatus = -1;

        SCLockThread::instance()->queryLock(cabintId,cellId, this);
       
        QEventLoop loop;
        connect(this, SIGNAL(signalProtoclResp()), &loop, SLOT(quit()),Qt::DirectConnection);
        
        loop.exec();
    
        return mstatus;
     }

    qint8 queryGoods(qint8 cabintId, qint8 cellId)
    {
        mstatus = -1;

        SCLockThread::instance()->queryGoods(cabintId,cellId, this);

        QEventLoop loop;
        connect(this, SIGNAL(signalProtoclResp()), &loop, SLOT(quit()),Qt::DirectConnection);

        loop.exec();

        return mstatus;
     }


signals:
    void signalProtoclResp();

    /*void  lockOperationCallback(qint8 cabintId, qint8 cellId,  qint8 status)
    {
        qDebug() << cabintId << cellId;
        mstatus = status;
        emit signalProtoclResp();
    }*/

protected:
    bool event(QEvent* e)
    { 
        CLockEvent* respEvent = (CLockEvent*)e;
        mstatus = respEvent->mStatus;
        emit signalProtoclResp();

        return QObject::event(e);
    }

private:

    qint8      mstatus;

  //  DECLARE_SINGLETON_CLASS(CLockDrv)

};

//typedef Singleton<CLockDrv> SCLockDrv;

#endif // LOCKDRV_H

