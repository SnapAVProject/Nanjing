#ifndef STATUS_H
#define STATUS_H


#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QTimer>

typedef enum{
    ST_NONE = 0x00,
    ST_HOME,                 //主页面
    ST_MANAGE,               //系统维修页面
    ST_INFO_INPUT,           //信息录入页面
    ST_PAKEAGE_CONFIRM,      //存件确认页面
    ST_NO_CARD_SAVE,         //无卡存件页面
    ST_SAVE_DELIVERY_MENU,   //存件主页面
    ST_SAVE_DELIVERY_WAIT,   //存件等待页面
    ST_SAVE_DELIVERY_LIST,   //存件列表页面
    ST_GET_DELIVERY,         //取件页面
    ST_SET_PORT,             //设置端口号界面
    ST_RECHARGE,             //充值
    ST_EXCEPTION_HANDLE,     //异常处理
    ST_EXCEPTION_OPENDOOR,   //异常处理、打开箱门
    ST_QUERY_BALANCE,        //余额查询
    ST_INTRO_LXYZ,           //易邮简介
    ST_PRICT_CHECK,          //资费查询
    ST_USE_GUIDE,            //使用流程
    ST_DEVICE_REST,          //设备维护中
    ST_FINANCE,              //财务页面
    ST_GET_CARD,             //刷卡取件
    ST_BIND_VIP,             //自助绑定
    ST_COURIER_TAKE,         //快递员取滞留件
    ST_CHECK_CARD,           //读取卡号
    ST_COURIER_HELP_TAKE     //快递员自助取件
}STATUS;

#define DEFAULT_TIMER_INTERVAL 120*1000
class CStatus : public QObject
{
    Q_OBJECT

public:
    CStatus(STATUS status) : mStatus(status)
    {
        timer.setInterval(DEFAULT_TIMER_INTERVAL);
        timer.setSingleShot(true);

        QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(timeOut()));
    }

    virtual void enter(QByteArray param) = 0;
    virtual void leave(QByteArray param) = 0;

    virtual void keyInputProc(QByteArray param) {qDebug()<<param;}
    virtual void cardInputProc(QByteArray param) {qDebug()<<param;}
    virtual void coinInputProc(QByteArray param) {qDebug()<<param;}
    virtual void upsInputProc(QByteArray param) {qDebug()<<param;}


    STATUS status()
    {
        return mStatus;
    }

    void openTimer()
    {
        //qDebug() << "start timer";
        timer.start();
    }

    void clearTimer()
    {
        timer.setInterval(DEFAULT_TIMER_INTERVAL);
    }

    void stopTimer()
    {
        timer.stop();
    }
/*
    void setTimeOutStatus(STATUS staus, QString defaulParam="")
    {
        timoutStatus = staus;
        mDefaultParam = defaulParam;
    }
*/
signals:
    void jumpPage_signal();

public slots:
    void timeOut()
    {
        //qDebug() << "end timer";
        emit jumpPage_signal();
        //SCMainCtrl::instance()->enterStatus(timoutStatus,"");
    }

private:
    STATUS  mStatus;    //当前页面标识
    QTimer  timer;
    STATUS  timoutStatus;
    QString mDefaultParam;
};











#endif // PAGE_H
