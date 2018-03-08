

#ifndef DRIVER_H
#define DRIVER_H


#include <QMutex>
#include <QEventLoop>
#include <QTimer>
#include <QProcess>
#include "server.h"
#include "singleton.h"




class CDriver : public QObject
{
    Q_OBJECT

public:

    void init(int serverPort);


    void onCommandProc(qint8 index, qint8 cmd, QByteArray data);

    bool openCellLock(qint8 cabinetId, qint8 cellId);

    bool queryCellLock(qint8 cabinetId, qint8 cellId, qint8* status);

    bool codeScan(QByteArray& data);

    bool screen(QByteArray& data);

    void playVoice(QString mp3File, QString volume);

    void catchVideo(QString);

    void setCheckGoods();

    bool readCheckGoods();

    void handleCodeScanQuit();



Q_SIGNALS:

    void keyInput(QByteArray  keyVal);
    void cardInput(QByteArray cardNo);
    void coinInput(QByteArray coinVal);
    void upsInput(QByteArray upsVal);

    void signalProtoclResp();
    void codeScanStop();

 public slots:

    void upsException();
    void deletimage();
    void repairUps();
    void upRepairUps();

private:

    QByteArray sendProtocl(qint8 cmd, QByteArray data, qint32 timeout = 0);



    CDriver();
    ~CDriver();

    QTimer upsTimer;

    Tcp::Server*   mServer;
    Tcp::Observer* mObserver;

    qint8          mWaitRespCommand;

    qint8          mRespCommand;
    QByteArray     mRespData;

    QMutex*        mMutex;
    QProcess*      mProcess;
    QProcess*      videoProcess;
    bool           isWorking;
    bool           isUpsWorking;  
    bool           photoStatus;
    bool           CheckGoods;
    DECLARE_SINGLETON_CLASS(CDriver)
};

typedef Singleton<CDriver> SCDriver;

#endif // DRIVER_H
