#ifndef SAVEIPTHREAD_H
#define SAVEIPTHREAD_H

#include <QThread>
#include <QObject>
#include <QTime>
#include <QDebug>
#include <QString>
#include <QHostAddress>
#include <QNetworkInterface>

#include "exceptioncomm.h"
#include "singleton.h"

#define GETIP_SLEEP_TIME  10*60

class saveIpThrd : public QThread
{
    Q_OBJECT
public:

    void init()
    {
        start();
    }
signals:


private:
    saveIpThrd(){}
    ~saveIpThrd(){}

    void run()
    {
        qsrand(time(0));
        ipAddrStr = qrand()%300;
        sleepTime = 10*60;
        while(1)
        {
            sleep(sleepTime);
            list.clear();
            list = QNetworkInterface::allAddresses();
            foreach (QHostAddress address, list)
            {
                if(address.protocol() == QAbstractSocket::IPv4Protocol)
                {
                    //IPv4地址
                    if (address.toString().contains("127.0."))
                    {
                        continue;
                    }
                    tempStr = address.toString();
                }
            }
            if (tempStr == "127.0.0.1")
            {
                qDebug() << "get local ip fail :" << tempStr;
            }
            else
            {
                qDebug() << "get local ip report time："<< tempStr << ipAddrStr << sleepTime;
                sleep(ipAddrStr);
                saveTerminalIpResp respone;
                SCExceptioncomm::instance()->saveTerminalIp(tempStr,&respone);

                if(respone.success() == RET_TRUE)
                {
                    sleepTime = respone.reportCycle().toInt();
                }
                else
                {
                    qDebug() << "set local ip fail";
                }
            }
        }
    }

    private:

    int sleepTime;
    int ipAddrStr;
    QList<QHostAddress> list;
    QString tempStr;

    DECLARE_SINGLETON_CLASS(saveIpThrd)
};

typedef Singleton<saveIpThrd> SSaveIpThrd;


#endif // SAVEIPTHREAD_H
