#ifndef MAINCTRL_H
#define MAINCTRL_H

#include <QObject>
#include <QMap>
#include <QDialog>
#include "status.h"
#include "singleton.h"


#define COIN_EXPIRY      5*60

class CMainCtrl : public QObject
{
    Q_OBJECT
public:


    void init();

    void enterStatus(STATUS status, QByteArray param);

    int curStatus();

    QMap<QString,qint8> expiryCoin;      //记录用户不按流程投币的投币数

signals:
    void updateCell_CMainCtrl();
    
public slots:

    void keyInputProc(QByteArray keyVal);
    void cardInputProc(QByteArray cardNo);
    void coinInputProc(QByteArray coinVal);
    void upsInputProc(QByteArray upsVal);

private:
    CMainCtrl();
    ~CMainCtrl();

    bool isworking;
    void addStatus(CStatus* status);

    STATUS                 mCurStatus;    //当前的状态
    QMap<STATUS, CStatus*> mStProcesser;  //状态处理者


    DECLARE_SINGLETON_CLASS(CMainCtrl)
};

typedef Singleton<CMainCtrl> SCMainCtrl;

#endif // MAINCTRL_H
