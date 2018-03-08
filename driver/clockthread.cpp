#include "clockthread.h"
#include "qextserialbase.h"
#include <QApplication>
#include <QTime>

CLockThread::CLockThread()
{

}


void CLockThread::init()
{
    mParserSt = ST_WAIT_FLAG_4C;
    comName = "/dev/ttyO5";
    myCom = new QextSerialPort(comName);
    myCom->open(QIODevice::ReadWrite);
    myCom->setBaudRate(BAUD19200);
    myCom->setDataBits(DATA_8);
    myCom->setParity(PAR_NONE);
    myCom->setStopBits(STOP_1);
    myCom->setFlowControl(FLOW_OFF);
    myCom->setTimeout(0,50);

}


void CLockThread::openLock(qint8 cabId, qint8 cellId, QObject* callback)
{
    CLockEvent* openEvt = new CLockEvent(OPEN_LOCK_TYPE, cabId, cellId, callback);
    QApplication::postEvent(this,openEvt);


}

void CLockThread::queryLock(qint8 cabId, qint8 cellId, QObject* callback)
{
    CLockEvent* queryEvt = new CLockEvent(QUERY_LOCK_TYPE, cabId, cellId, callback);
    QApplication::postEvent(this,queryEvt);
}


void CLockThread::queryGoods(qint8 cabId, qint8 cellId, QObject *callback)
{
    CLockEvent* goodsEvt = new CLockEvent(QUERY_GOODS_TYPE, cabId, cellId, callback);
    QApplication::postEvent(this,goodsEvt);
}

/**********************************************************************
* 函数名称： appointPolicy
* 功能描述： 组合成约定协议
* 输入参数：
* 输出参数：
* 修改日期        版本号     修改人	      修改内容
* -----------------------------------------------
* 2013/09/22	 V1.0
***********************************************************************/
QByteArray CLockThread::appointPolicy(LOCK_CMD cmd, qint8 cabinetId, QByteArray data)
{
    qint8      check = 0;
    QByteArray protocol;

    //数据区超大
    if (data.size() > 0xFF)
        return NULL;

    protocol.append('L');
    protocol.append('X');
    protocol.append(cabinetId);
    check += cabinetId;
    protocol.append(cmd);
    check += cmd;
    protocol.append(data.size());
    check += data.size();

    for (int i = 0; i < data.size(); i++)
    {
        check += data[i];
        protocol.append(data[i]);
    }
    protocol.append(check);
    protocol.append('E');

    return protocol;
}




/**********************************************************************
* 函数名称： queryLockResponse
* 功能描述： queryLockResponse 查询锁事件
* 输入参数：
* 输出参数：
* 修改日期        版本号     修改人	      修改内容
* -----------------------------------------------
* 2013/08/07	 V1.0
***********************************************************************/

bool CLockThread::event(QEvent *event)
{
    qint8 status = 0;
    if(event->type() == OPEN_LOCK_TYPE)
     {
        CLockEvent *myEvent = static_cast<CLockEvent *>(event);
        bool ret;
        QByteArray data;
        QByteArray recvdata;
        data.append(myEvent->mCellId);
        QByteArray retData = appointPolicy(LX_OPEN_LOCK,myEvent->mCabId,data);

        int i = 0;
        int count = 0;
        do
        {
            ret = false;
            while(count < 3)
            {
                //send open frame
                myCom->write(retData);
                qDebug() << "open senddata---------:" << retData.toHex();

                //recv open fram
                //delayTime(20);
                usleep(20000);
                recvdata = myCom->readAll();
                qDebug() << "open recvdata-----------" << recvdata.toHex() << "i:" << i;

                for (int k = 0; k < recvdata.size() ; k++ )
                {
                    if (onCharProc(recvdata[k]))
                    {
                        if ((mCommand == 0x02) && (mLength == 0))
                        {
                            count = 3;
                            break;
                        }
                    }
                }

                count++;
                qDebug() << "open count:------------" << count;
            }
            count = 0;
            // qDebug() << "myCom is write openLock:" << retData.toHex();
            //wait 1000
            // delayTime(1000);
            sleep(1);
            //query lock status
            ret = queryLockProc(myEvent->mCabId, myEvent->mCellId, &status);
            if (ret && status)
            {
                break;
            }
        }while(++i < 3);
   	
        CLockEvent resp(OPEN_LOCK_TYPE, myEvent->mCabId, myEvent->mCellId,NULL, status);
        QApplication::sendEvent(myEvent->callback, &resp);

        //myEvent->callback(myEvent->mCabId,myEvent->mCellId,status);
       // emit lockProtoclResp(myEvent->mCabId,myEvent->mCellId,status); //status 0: close 1: open -1 : error
          // myEvent.

           // emit openLockSignal(true);

    }
    else if (event->type() == QUERY_LOCK_TYPE)
     {
         CLockEvent *myEvent = static_cast<CLockEvent *>(event);

         if (!queryLockProc(myEvent->mCabId, myEvent->mCellId, &status))
             status = -1;


         CLockEvent resp(QUERY_LOCK_TYPE, myEvent->mCabId, myEvent->mCellId,NULL, status);
         QApplication::sendEvent(myEvent->callback, &resp);
         //emit lockProtoclResp(myEvent->mCabId,myEvent->mCellId,status);
         //myEvent->callback(myEvent->mCabId,myEvent->mCellId,status);
     }
    else if(event->type() == QUERY_GOODS_TYPE)
    {
        CLockEvent *myEvent = static_cast<CLockEvent *>(event);

        if (!queryGoodsProc(myEvent->mCabId, myEvent->mCellId, &status))
            status = -1;


        CLockEvent resp(QUERY_LOCK_TYPE, myEvent->mCabId, myEvent->mCellId,NULL, status);
        QApplication::sendEvent(myEvent->callback, &resp);
    }
     return  QObject::event(event);
 }


bool CLockThread::queryLockProc(qint8 cabinetId,qint8 cellId, qint8* status )
{
    //clear
   // myCom->readAll();
    int count = 0;
    bool ret = false;
    //send query frame
    QByteArray data;
    QByteArray qureyData;
    qureyData.append(cellId);
    QByteArray retData = appointPolicy(LX_QUERY_LOCK,cabinetId,qureyData);

    while(count < 3)
    {
        myCom->write(retData);
        qDebug() << "query senddata++++++++++++++++:" << retData.toHex();
        //wait 20ms
        //delayTime(20);
        usleep(20000);
        //read recv buffer
        data =  myCom->readAll();
        qDebug() << "query recvdata++++++++++++++++:" << data.toHex();
        //parser recv frame
        for (int i = 0; i < data.size() ; i++)
        {
            if (onCharProc(data[i]))
            {
                if ((mCommand == 0x04) &&  (mAddr == cabinetId) && (mLength == 2) && (mData[0] == cellId))
                {
                    *status = mData[1];
                    ret = true;
                    count = 3;
                    break;
                }
            }
        }
        count++;
        qDebug() << "query count:+++++++++++++++++" << count;
    }
    return ret;
}


bool CLockThread::queryGoodsProc(qint8 cabinetId,qint8 cellId, qint8* status)
{
    int count = 0;
    bool ret = false;
    //send query frame
    QByteArray data;
    QByteArray qureyData;
    qureyData.append(cellId);
    QByteArray retData = appointPolicy(LX_QUERY_GOODS,cabinetId,qureyData);

    while(count < 3)
    {
        myCom->write(retData);
        qDebug() << "query senddata++++++++++++++++:" << retData.toHex();
        //wait 20ms
        //delayTime(20);
        usleep(500000);
        //read recv buffer
        data =  myCom->readAll();
        qDebug() << "query recvdata++++++++++++++++:" << data.toHex();
        //parser recv frame
        for (int i = 0; i < data.size() ; i++)
        {
            if (onCharProc(data[i]))
            {
                if ((mCommand == 0x06) &&  (mAddr == cabinetId) && (mLength == 2) && (mData[0] == cellId))
                {
                    *status = mData[1];
                    ret = true;
                    count = 3;
                    break;
                }
            }
        }
        count++;
        qDebug() << "query count:+++++++++++++++++" << count;
    }
    return ret;
}










