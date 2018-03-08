#ifndef SENDFILE_H
#define SENDFILE_H

#include <QThread>
#include "urlencode.h"
#include "singleton.h"
#include "servercomm.h"
#include "mainctrl.h"
#include "db.h"
#include <QTime>
#include <QDebug>

#define FILE_SLEEP_TIME  35*60

class CSendFileThrd : QThread
{
    Q_OBJECT

public:

    void init()
    {
        start();
    }


private:
    CSendFileThrd(){}
    ~CSendFileThrd(){}

    bool performStatus;
    QString terminal;

    void writeAdminCard()
    {
        QFile f("IPSetCard.txt");
        if(f.open(QIODevice::ReadWrite))
        {
            QTextStream out(&f);
            if(out.readLine() == "Version1")
            {

            }
            else
            {
                f.resize(0);
                out << "Version1" << endl;
                out << "2957348563" << endl;   out << "2956980707" << endl;
                out << "2860716371" << endl;   out << "2856744003" << endl;
                out << "2860725171" << endl;   out << "2856741699" << endl;
                out << "2860531539" << endl;   out << "2856731331" << endl;
                out << "2860000883" << endl;   out << "2856577715" << endl;
                out << "2856752003" << endl;   out << "2860456275" << endl;
                out << "2860605043" << endl;   out << "2856484227" << endl;
                out << "2856432691" << endl;   out << "2860407875" << endl;
                out << "2856927971" << endl;   out << "2856311635" << endl;
                out << "2856884851" << endl;   out << "2856363251" << endl;
                out << "2856883059" << endl;   out << "2856902531" << endl;
                out << "2856974867" << endl;   out << "2860630867" << endl;
                out << "2859872851" << endl;   out << "2856454403" << endl;
                out << "2860510115" << endl;   out << "2856515651" << endl;
                out << "2860409555" << endl;   out << "2856535267" << endl;
                out << "2856836915" << endl;   out << "2859950931" << endl;
                out << "2860437571" << endl;   out << "2860103811" << endl;
                out << "2857045843" << endl;   out << "2857182691" << endl;
                out << "2856910515" << endl;   out << "2857151379" << endl;
                out << "2859872547" << endl;   out << "2856667779" << endl;
                out << "2856325155" << endl;   out << "2856505587" << endl;
                out << "2857058195" << endl;   out << "2860121619" << endl;
                out << "2860425619" << endl;   out << "2856973667" << endl;
                out << "2856335251" << endl;   out << "2857048723" << endl;
                out << "2856625891" << endl;   out << "2856979075" << endl;

                out << "2860727571" << endl;   out << "2857102003" << endl;
                out << "2860587715" << endl;   out << "2860530451" << endl;
                out << "2856846739" << endl;   out << "2860061331" << endl;
                out << "2856351091" << endl;   out << "2860487779" << endl;
                out << "2860221971" << endl;   out << "2859912227" << endl;
                out << "2856592435" << endl;   out << "2860376547" << endl;
                out << "2860715811" << endl;   out << "2859879011" << endl;
                out << "2860672915" << endl;   out << "2856672291" << endl;
                out << "2857171667" << endl;   out << "2856422515" << endl;
                out << "2856794339" << endl;   out << "2856423283" << endl;
            }
        }
         f.close();
    }

    void sendFileProc()
    {

        QTime time = QTime::currentTime();
        int hour = time.hour();
        QString fileName = "debuglog.txt";

        if(3 != hour)
        {
            performStatus = true;
        }
        if((3 == hour) && performStatus)
        {

        //	SCDatBase::instance()->deleteTable();
            writeAdminCard();
            int filetime = qrand()%20;
            qDebug() << "start send file" << filetime;
            sleep(filetime*60);
            QFile filesize("debuglog.txt");

            if(filesize.size() > 10*1024*1024)
            {
                filesize.resize(0);
            }       

           uploadDeliveryPhotoResp respone;
           SCServerComm::instance()->uploadDebugFiles(fileName,&respone);
           if(respone.success() == RET_TRUE)
           {
              performStatus = false;
              filesize.resize(0);
           }
        }
    }

    void run()
    {
        qsrand(time(0));
        performStatus = true;
        while(1)
        {

            sleep(FILE_SLEEP_TIME);
            terminal = SCDatBase::instance()->getTerminalId();

            if ((SCMainCtrl::instance()->curStatus() == 1) && terminal!="0000000000000000")
            {
                sendFileProc();
            }
        }
    }
    DECLARE_SINGLETON_CLASS(CSendFileThrd)
};

typedef Singleton<CSendFileThrd> SCSendFileThrd;
#endif // SENDFILE_H
