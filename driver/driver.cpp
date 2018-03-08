#include <QDebug>
#include <QByteArray>
#include <QEventLoop>
#include <QStringList>
#include <QString>
#include <QProcess>
#include <QDateTime>
#include "lxprotocol.h"
#include "driver.h"
#include "db.h"
#include "exceptioncomm.h"
#include "sendimage.h"

#include <QDir>



using namespace Tcp;

extern qint8 gIndex;

CDriver::CDriver()
{
    mServer = new Tcp::Server;
    mObserver = new LxProtocolParser;
    mServer->addObserver(mObserver);
    mMutex  = new QMutex;

    mWaitRespCommand = 0;
    mRespCommand     = 0;
    isWorking = false;
    isUpsWorking = false;

    /*
    QDir dir("/lxyz/image/");

    QStringList nameFilter;
    nameFilter << "*.gif";
    QStringList images = dir.entryList(nameFilter, QDir::NoFilter, QDir::Time);

    for (int i = 0; i < images.size(); i++)
        postFile(url ,  "/lxyz/image/" + images[i]);
*/
}



CDriver::~CDriver()
{
    delete mServer;
    delete mObserver;
    mProcess->close();
    videoProcess->close();
    delete mProcess;
    delete videoProcess;
}

void CDriver::init(int serverPort)
{
    mServer->start(serverPort);
    mProcess = new QProcess;
    videoProcess = new QProcess;

    CheckGoods = SCDatBase::instance()->getGoodsCheck();

    connect(this,SIGNAL(upsInput(QByteArray)),this,SLOT(upsException()));
    connect(&upsTimer,SIGNAL(timeout()),this,SLOT(repairUps()));
    connect(videoProcess,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(deletimage()));

}


void CDriver::onCommandProc(qint8 index, qint8 cmd, QByteArray data)
{
    mRespCommand = cmd;
    mRespData = data;

  //  qDebug() << "index: "<< index << "cmd: " << cmd << "data: " << data.toHex();

    if(cmd == mWaitRespCommand + 1 && gIndex == index)
        emit signalProtoclResp();

    switch(cmd){
    case LX_KEY_IPUT:
        emit keyInput(data);
        break;
    case LX_M1_CARD_INPUT:
        emit cardInput(data);
        break;
    case LX_COIN_INPUT:
        emit coinInput(data);
        break;
    case LX_UPS_INPUT:
        emit upsInput(data);
        break;
    }
}




bool CDriver::openCellLock(qint8 cabinetId, qint8 cellId)
{
    QByteArray openLockData;
    QByteArray respData;

    openLockData.append(cabinetId);
    openLockData.append(cellId);

    respData = sendProtocl((qint8)LX_OPEN_LOCK, openLockData, 6000);

    qDebug()<< " open lock respone: " << respData.toHex();

    if (respData.size() != 3)
    {
        qDebug() << "open lock timeout error";
        return false;
    }


    if (cabinetId == respData[0]  && cellId == respData[1] && 1 == respData[2] )   //respData[2]判断打开的状态
    {
        return  true;
    }
    else
    {
        qDebug() << "open lock status error";
        return false;
    }
}


bool CDriver::queryCellLock(qint8 cabinetId, qint8 cellId, qint8* status)
{

    QByteArray openLockData;
    QByteArray respData;

    openLockData.append(cabinetId);
    openLockData.append(cellId);

    respData = sendProtocl((qint8)LX_QUERY_LOCK, openLockData, 8000);

    if (respData.size() != 3)
    {
        return false;
    }

    *status = respData[2];

    if (cabinetId == respData[0]  && cellId == respData[1] )
    {
        return  true;
    }
    else
    {
        return false;
    }
}


void CDriver::handleCodeScanQuit()
{
    emit codeScanStop();
}

bool CDriver::codeScan(QByteArray& data)
{
    QByteArray respData = sendProtocl((qint8)LX_CODE_SCAN, QByteArray(), 30000);

//    qDebug() << "driver respData:" << respData.toHex();
    if (respData.size() < 6)
    {
        return false;
    }

    data = respData;

    return  true;
}

bool CDriver::screen(QByteArray &data)
{
    QByteArray respData = sendProtocl((qint8)LX_SCREEN, QByteArray(), 5000);

    if (!respData.size())
    {
        return false;
    }

    data = respData;

    return  true;
}


QByteArray CDriver::sendProtocl(qint8 cmd, QByteArray data, qint32 timeout)
{
    QByteArray protocolData;
    QEventLoop loop;
    QTimer     timer;

    if (!isWorking)
   {
    //mMutex->lock();

	isWorking = true;

    mRespData.clear();

    mWaitRespCommand = cmd;

    protocolData = LxProtocolGenerater::generate((LX_CMD)cmd, data);
    mServer->write(protocolData);

    if (timeout)
    {
        timer.setSingleShot(true);
        timer.setInterval(timeout);
        timer.start();
    }


    // 进入事件循环， 直到超时事件或者接收到回应
    connect(this, SIGNAL(signalProtoclResp()), &loop, SLOT(quit()));
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(this,SIGNAL(codeScanStop()),&loop,SLOT(quit()));

    loop.exec();


    mWaitRespCommand = 0;

    if (timeout) {
        timer.stop();
    }
	isWorking = false;
     return mRespData;
   }
   
  //mMutex->unlock();
    mRespData.clear();
    return mRespData;

}


void CDriver::upsException()
{

    if(!isUpsWorking)
    {       

        isUpsWorking = true;
        //上报异常

        QString type = "1";

        QDateTime curTime = QDateTime::currentDateTime();
        QString str = curTime.toString("yyyy-MM-dd hh:mm:ss");

        //记录异常
        QString upsTime;
        SCDatBase::instance()->getUpsInfo(&upsTime);
        if(upsTime.contains("true"))
        {
            reportCommonErrorsResp respone;
            SCExceptioncomm::instance()->reportCommonErrors(type,&respone);

            if(respone.success() == RET_TRUE)
            {

            }

            QString upsInfo = "false" + str;
            SCDatBase::instance()->setUpsInfo(upsInfo);
        }

        upsTimer.setInterval(5*60*1000);
        upsTimer.setSingleShot(true);
        upsTimer.start();


    }
    else
    {
        upsTimer.setInterval(5*60*1000);
    }
}

void CDriver::repairUps()
{
    isUpsWorking = false;

    //获取异常发生时间
    QString upsTime;
    SCDatBase::instance()->getUpsInfo(&upsTime);
    upsTime.remove(0,5);

    //上报修复
    QString type = "1";
    //修复ups异常
    QString upsInfo = "true";
    SCDatBase::instance()->setUpsInfo(upsInfo);

    repairCommonErrorsResp respone;
    SCExceptioncomm::instance()->repairCommonErrors(type,upsTime,&respone);
    if(respone.success() == "true")
    {

    }

    upsTimer.stop();
}


void CDriver::upRepairUps()
{
    if(!isUpsWorking)
    {
        //获取异常发生时间
        QString upsTime;
        SCDatBase::instance()->getUpsInfo(&upsTime);
        upsTime.remove(0,5);

        //上报修复
        QString type = "1";

        QString upsStatus = "true";
        SCDatBase::instance()->setUpsInfo(upsStatus);

        repairCommonErrorsResp respone;
        SCExceptioncomm::instance()->repairCommonErrors(type,upsTime,&respone);
        if(respone.success() == "true")
        {

        }
    }
}


void CDriver::deletimage()
{
    if(photoStatus)
    {
        system("rm ./picture/*.bmp");
    }
}


void CDriver::playVoice(QString mp3File, QString volume)
{
    QStringList list;
    list.append(mp3File);
    list.append("-A");
    list.append(volume);
    mProcess->start("madplay", list);

   // mProcess->start("mplayer", list);
    //QProcess::execute("madplay", list);
}


void CDriver::catchVideo(QString imageFile)
{
    if(SCSendImageThrd::instance()->readSendImageStatus())
    {
        QStringList list;

        if(imageFile == "test")
        {
            photoStatus = false;
        }
        else
        {
            photoStatus = true;
        }

        imageFile = "/usr/lxyz/picture/" + imageFile + ".gif";
        list.append(imageFile);
        videoProcess->start("capture",list);
    }
}

void CDriver::setCheckGoods()
{
    CheckGoods = !CheckGoods;
}

bool CDriver::readCheckGoods()
{
    return CheckGoods;
}


