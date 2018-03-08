#ifndef CLOCKTHREAD_H
#define CLOCKTHREAD_H

#include <QDialog>
#include "qextserialport.h"
#include "singleton.h"
#include <QThread>
#include <QEvent>
#include <QTimer>
#include <QEventLoop>
#include <QDebug>

typedef enum {
    LX_OPEN_LOCK  = 0x01,
    LX_OPEN_LOCK_RESP,
    LX_QUERY_LOCK,
    LX_QUERY_LOCK_RESP,
    LX_QUERY_GOODS,
    LX_QUERY_GOODS_RESP
}LOCK_CMD;

namespace Ui {
class CLockThread;
}

static const QEvent::Type OPEN_LOCK_TYPE = (QEvent::Type)QEvent::registerEventType(QEvent::User + 1); //开锁
static const QEvent::Type QUERY_LOCK_TYPE = (QEvent::Type)QEvent::registerEventType(QEvent::User + 2); //查询锁
static const QEvent::Type QUERY_GOODS_TYPE = (QEvent::Type)QEvent::registerEventType(QEvent::User + 3); //查询物品

class CLockEvent : public QEvent
{
public:
    CLockEvent(QEvent::Type type, qint8 cabinet, qint8 cell, QObject* cb, qint8 status = -1):
        QEvent(type),
        mCabId(cabinet),
        mCellId(cell),
        callback(cb),
        mStatus(status)
    {
    }

public:
    qint8 mCabId;
    qint8 mCellId;
    QObject* callback;
    qint8 mStatus;
};



class CLockThread : public QObject
{
    Q_OBJECT
    
public:

    void init();
    void openLock(qint8 cabId, qint8 cellId, QObject* callback);
    void queryLock(qint8 cabId, qint8 cellId, QObject* callback);
    void queryGoods(qint8 cabId,qint8 cellId, QObject* callback);

signals:
    void lockProtoclResp(qint8,qint8,qint8);


private:

    CLockThread();

    bool queryLockProc(qint8, qint8 , qint8* );
    bool queryGoodsProc(qint8,qint8,qint8*);


    QByteArray appointPolicy(LOCK_CMD cmd,qint8 cabinetId,QByteArray data);


    void delayTime(long time)
    {
        QTimer timer;
        QEventLoop loop;
        timer.setInterval(time);
        timer.setSingleShot(true);
        timer.start();

        connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
        loop.exec();
    }

    void onDataProc(QByteArray data)
    {
        qDebug() << "input data : " <<  data.toHex() ;

        for (int i = 0; i < data.size(); i++)
            onCharProc(data[i]);

    }


    bool onCharProc(qint8 c)
    {
        switch (mParserSt)
        {
            case ST_WAIT_FLAG_4C:
                if ( 0x4c == c)
                    mParserSt = ST_WAIT_FLAG_58;
                break;
            case ST_WAIT_FLAG_58:
                if (0x58 == c)
                {
                    mParserSt = ST_WAIT_INDEX;
                    mCheck = 0;
                    mCurPos = 0;
                    mLength = 0;
                }
                else
                {
                    mParserSt = ST_WAIT_FLAG_4C;
                }
                break;
            case ST_WAIT_INDEX:
                mAddr = c;
                mParserSt = ST_WAIT_CMD;
                mCheck += c;
                break;
            case ST_WAIT_CMD:
                mCommand = c;
                mCheck += c;
                mParserSt = ST_WAIT_LENGTH;
                break;
            case ST_WAIT_LENGTH:
                mLength = c;
                mCheck += c;
                if (!mLength)
                {
                    mParserSt = ST_WAIT_CHECK;
                }
                else
                {

                    mParserSt = ST_WAIT_DATA;
                }
                break;
            case ST_WAIT_DATA:
                mData[mCurPos++] = c;
                mCheck += c;
                if (mCurPos >= mLength)
                {
                    mParserSt = ST_WAIT_CHECK;
                }
                break;
            case ST_WAIT_CHECK:
                if (mCheck == c)
                {
                    mParserSt = ST_WAIT_FLAG_45;
                }
                else
                {
                    mParserSt = ST_WAIT_FLAG_4C;
                }
                break;
            case ST_WAIT_FLAG_45:
                if (c == 0x45)
                {
                    //处理数据帧
                    //QByteArray data((const char*)mData, mLength);
                    //onCommandProc(mIndex, mCommand,  data);

                    mParserSt = ST_WAIT_FLAG_4C;
                    return true;


                }
                mParserSt = ST_WAIT_FLAG_4C;
                break;
            default:
                break;
            }

        return false;
    }





protected:
    bool event(QEvent *);
    
private:
    QString comName;
    QextSerialPort *myCom;

    typedef enum {
        ST_WAIT_FLAG_4C, //初始状态，等待接收0x4c
        ST_WAIT_FLAG_58, //已经接收到0x4c，等待接收0x58
        ST_WAIT_INDEX,   //等待结算INDEX
        ST_WAIT_CMD,     //已经接收到INDEX，等待接收CMD
        ST_WAIT_LENGTH,  //已经接收到CMD，等待接收LENGTH
        ST_WAIT_DATA,    //已经接收到LENGTH，接收数据
        ST_WAIT_CHECK,   //数据接收完成，等待接收校验码
        ST_WAIT_FLAG_45  //等待接收结束标识
    }PARSER_ST;

    PARSER_ST  mParserSt; //字符状态机当前状态
    qint8      mCommand;  //接收到命令符
    qint8      mLength;   //数据区长度
    qint8      mCurPos;   //当前接收到数据位置
    qint8      mData[256];//接收到的数据缓冲
    qint8      mCheck;    //校验累加和
    qint8      mAddr;    //地址 箱号
   

    DECLARE_SINGLETON_CLASS(CLockThread)
};

typedef Singleton<CLockThread> SCLockThread;

#endif // CLOCKTHREAD_H
