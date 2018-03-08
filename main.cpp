
#include <QApplication>
#include <QDebug>
#include <QTextCodec>
#include <QThread>
#include <QIcon>
#include <QTime>
#include <QObject>
#include "servercomm.h"
#include "exceptioncomm.h"
#include "mainctrl.h"
#include "driver.h"
#include "respone.h"
#include "clockthread.h"
#include "configure/db.h"
#include "exception.h"
#include "watchdog.h"
#include "sendimage.h"
#include "sendfile.h"
#include "update/updatesystem.h"
#include "runscript.h"
#include "saveIPThread.h"
#include <QWSServer>

void customMessageHandler(QtMsgType type, const char *msg)
{
     QTime current_time = QTime::currentTime();
     QString txt;
     switch (type) {
        //调试信息提示
       case QtDebugMsg:
               txt = QString("Debug: %1").arg(msg);
              break;

       //一般的warning提示
       case QtWarningMsg:
               txt = QString("Warning: %1").arg(msg);
       break;
       //严重错误提示
       case QtCriticalMsg:
               txt = QString("Critical: %1").arg(msg);
       break;
       //致命错误提示
      case QtFatalMsg:
               txt = QString("Fatal: %1").arg(msg);
               abort();
       }

      txt = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd") +QString(":%1").arg(current_time.msec()) + txt;
      QFile outFile("debuglog.txt");
      outFile.open(QIODevice::WriteOnly | QIODevice::Append);
      QTextStream ts(&outFile);
      ts << txt << endl;
       if(outFile.size() > 10*1024*1024)
       {
         outFile.resize(0);
       }


}


void repairUps()
{
    //修复ups异常
    QString upsInfo;
    SCDatBase::instance()->getUpsInfo(&upsInfo);

    if(upsInfo.contains("false"))
    {

        QTimer *timer = new QTimer;
        timer->setInterval(8*60*1000);
        timer->setSingleShot(true);
        timer->start();

        QObject::connect(timer,SIGNAL(timeout()),SCDriver::instance(),SLOT(upRepairUps()));

    }
}



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    QWSServer::setCursorVisible(false);

    //a.setOverrideCursor(QCursor(Qt::BlankCursor));
    //a.setStyleSheet("QPushButton {border:2px groove gray;border-radius:10px;padding:2px 4px;}");
    //添加默认图标
   //  a.setStyleSheet("QLineEdit{padding: 1px;border-style: solid;border: 2px solid gray;border-radius: 8px;}");
   // 先注册自己的MsgHandler
    qInstallMsgHandler(customMessageHandler);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    //喂狗线程
    Watchdog wtdg;
    wtdg.start();

    //数据库模块初始化
    SCDatBase::instance()->init("./portalDb.sqlite");

    //驱动初始化
    SCDriver::instance()->init(8089);

    //主控制模块初始化
    SCMainCtrl::instance()->init();

    //通信模块初始
    QString ip , port ;
    SCDatBase::instance()->getServerInfo(&ip, &port);
    SCServerComm::instance()->init(ip, port);
    SCExceptioncomm::instance()->init();

    //ups断电修复
    repairUps();

   //打开、查询箱门线程
    SCLockThread::instance()->init();
    QThread lockOperation;
    SCLockThread::instance()->moveToThread(&lockOperation);
    lockOperation.start();

    //异常处理线程
    SExceptionThrd::instance()->init();

    //传图片线程
    SCSendImageThrd::instance()->init();

    //传文件线程
    SCSendFileThrd::instance()->init();

    //远程更新
    SCUpdateSystemThrd::instance()->init();

    //监测网络
    SRunScriptThrd::instance()->init();

    //时时上传IP地址
    SSaveIpThrd::instance()->init();

    QObject::connect(SExceptionThrd::instance(),SIGNAL(updateCell_CExceptionThrd()),SCMainCtrl::instance(),SIGNAL(updateCell_CMainCtrl()));

    system("syslogd -O /usr/lxyz/debuglog.txt -s 15000"); //Klogd的输出结果会传送给syslogd进行处理,syslogd会根据/etc/syslog.conf的配置把log信息输出到/var/log/下

    system("klogd -n /usr/lxyz/debuglog.txt &");

    system("sh qtcheck_daemon.sh &");
    return a.exec();
}
