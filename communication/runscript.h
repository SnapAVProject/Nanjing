#ifndef RUNSCRIPT_H
#define RUNSCRIPT_H

#include <QThread>
#include <QObject>
#include <QTime>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QString>

#include "singleton.h"
#include "mainctrl.h"
#include "configure/mconfig.h"
#include "configure/mconfig.h"

#define CHECKNET_SLEEP_TIME  30
#define ERROR_TIMES        30
class runScriptThrd : public QThread
{
    Q_OBJECT
public:

    void init()
    {
        start();
    }

signals:


private:
    runScriptThrd(){}
    ~runScriptThrd(){}

    void run()
    {
        int returnValue = 0;
        int errorTimes = 0;
        QString pingAddrStr;
        QString pingIP;

        QTime temptime = QTime::currentTime();
        QString networklevel ;

        while(1)
        {
            sleep(CHECKNET_SLEEP_TIME);
            Smconfig::instance()->readPingIP(pingIP);
            //qDebug() << "runScriptThrd pingIP" << pingIP;
            pingAddrStr = "ping -c 1 -s 0 "+ pingIP +" > /dev/null";
            returnValue = system(pingAddrStr.toStdString().c_str());
            if (WEXITSTATUS( returnValue ) == 0)
            {
                errorTimes = 0;
            }
            else
            {
                errorTimes++;
                qDebug() << "errorTimes " << errorTimes;

                Smconfig::instance()->readNetworklevel(networklevel);
                if(networklevel == "0")
                {
                    system("(sh callcdma.sh &) >> debuglog.txt");
                }
            }

            if ((SCMainCtrl::instance()->curStatus() == 1))
            {
                if(errorTimes >= ERROR_TIMES)
                {
                    qDebug() << "runScriptThrd start reboot errorTimes" << errorTimes ;
                    errorTimes = 0;
                    system("reboot");
                }
                //获取当前系统时间判断是否需要重启
                temptime = QTime::currentTime();
                if((temptime.hour()==4)&&(temptime.minute()>=59))
                {
                    system("sed -i '/pppd call cdma/d' /etc/init.sh");
                    system("sed -i '/ifconfig eth0 up/d' /etc/init.sh");
                    system("sed -i '/udhcpc/d' /etc/init.sh");

                    qDebug() << "system reboot time:" << temptime;
                    system("reboot");
                }
            }
        }
    }


    DECLARE_SINGLETON_CLASS(runScriptThrd)
};

typedef Singleton<runScriptThrd> SRunScriptThrd;

#endif // RUNSCRIPT_H
