#include "mainctrl.h"
#include "homestatus.h"
#include "takepackagestatus.h"
#include "noiclogstatus.h"
#include "infoinputstatus.h"
#include "managestatus.h"
#include "waitconfirmstatus.h"
#include "savepackagestatus.h"
#include "savepackagewaitstatus.h"
#include "savepackageliststatus.h"
#include "setportstatus.h"
#include "driver.h"
#include "exception.h"
#include "rechargestatus.h"
#include "querybalancestatus.h"
#include "useguidestatus.h"
#include "cintroductionlxyzstatus.h"
#include "pricecheckstatus.h"
#include "exceptionstatus.h"
#include "exceptionopendoorstatus.h"
#include "couriertakestatuse.h"
#include "devicereststatus.h"
#include "financestatus.h"
#include "takeliststatus.h"
#include "vipbindstatus.h"
#include "cardcheckstatus.h"
#include "exphelpstatus.h"

CMainCtrl::CMainCtrl()
{
    isworking = false;
}

CMainCtrl::~CMainCtrl()
{

    QMap<STATUS, CStatus*>::iterator it;
    for (it = mStProcesser.begin(); it != mStProcesser.end(); ++it)
        delete it.value();

}

void CMainCtrl::init()
{
    mCurStatus = ST_NONE;


    //添加各个状态
    CHomeStatus*  homeStatus = new CHomeStatus;                            //主界面
    CManageStatus  *managestatus = new CManageStatus;                      //管理员
    CInfoinputStatus *infoputstatus = new CInfoinputStatus;                //信息录入
    CWaitconfirmStatus *waitconfirmstatus = new CWaitconfirmStatus;        //等待确认界面
    CNoiclogStatus  *noiclogstatus = new CNoiclogStatus;                   //无卡登录
    CSavepackageStatus * savestatus = new CSavepackageStatus;              //存件界面
    CSavepackagewaitStatus *savewaitstatus = new CSavepackagewaitStatus;   //存件等待页面
    CSavepackagelistStatus *saveliststatus = new CSavepackagelistStatus;   //存件列表页面
    CTakePageStatus *takepagestatus = new CTakePageStatus;                 //取件
    CSetportstatus *portstatus = new CSetportstatus;                       //设置端口
    CRechargeStatus *rechargestatus = new CRechargeStatus;                 //充值
    CQueryBalanceStatus *balancestatus = new CQueryBalanceStatus;          //余额查询
    CUseguideStatus *useguidestatus = new CUseguideStatus;                 //使用流程
    CIntroductionLXYZStatus *introLxyzstatus = new CIntroductionLXYZStatus;  //易邮简介
    pricecheckstatus      *priceCheckStatus = new pricecheckstatus;         //资费查询
    CExceptionStatus *exceptionstatus = new CExceptionStatus;                           //异常处理
    CExceptionOpenDoorStatus *exceptionopendoorstatus = new CExceptionOpenDoorStatus;   //异常打开箱门
    CourierTakePackageStatus *courierpackagestatus = new CourierTakePackageStatus;      //快递员取滞留件
    CDeviceRestStatus *devicereststatus = new CDeviceRestStatus;           //设备维护
    CFinanceStatus *financestatus = new CFinanceStatus;                    //财务界面
    CTakeListStatus *takeliststatus = new CTakeListStatus;                 //刷卡取件
    CVipBindStatus *bindstatus = new CVipBindStatus;                       //自助绑定
    CCardcheckStatus *cardstatus = new CCardcheckStatus;                   //刷卡显示
    CExpHelpStatus *exphelpstatus = new CExpHelpStatus;                    //快递员取件


    addStatus(homeStatus);
    addStatus(managestatus);
    addStatus(infoputstatus);
    addStatus(waitconfirmstatus);
    addStatus(noiclogstatus);
    addStatus(savestatus);
    addStatus(savewaitstatus);
    addStatus(saveliststatus);
    addStatus(takepagestatus);
    addStatus(portstatus);
    addStatus(rechargestatus);
    addStatus(balancestatus);
    addStatus(useguidestatus);
    addStatus(introLxyzstatus);
    addStatus(priceCheckStatus);
    addStatus(exceptionstatus);
    addStatus(exceptionopendoorstatus);
    addStatus(courierpackagestatus);
    addStatus(devicereststatus);
    addStatus(financestatus);
    addStatus(takeliststatus);
    addStatus(bindstatus);
    addStatus(cardstatus);
    addStatus(exphelpstatus);
    //......


    expiryCoin.insert(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),0);
    //读取背景图片位置，
    enterStatus(ST_HOME, "REFRESHCELL");

    connect(this,SIGNAL(updateCell_CMainCtrl()),homeStatus,SLOT(updateCell()));

    //绑定驱动事件
    connect(SCDriver::instance(), SIGNAL(keyInput(QByteArray)), this, SLOT(keyInputProc(QByteArray)));
    connect(SCDriver::instance(), SIGNAL(cardInput(QByteArray)), this, SLOT(cardInputProc(QByteArray)));
    connect(SCDriver::instance(), SIGNAL(coinInput(QByteArray)), this, SLOT(coinInputProc(QByteArray)));
    connect(SCDriver::instance(), SIGNAL(upsInput(QByteArray)), this, SLOT(upsInputProc(QByteArray)));

}

void CMainCtrl::addStatus(CStatus *status)
{
     mStProcesser.insert(status->status(), status);

}
int CMainCtrl::curStatus()
{
    return mCurStatus;
}

void CMainCtrl::enterStatus(STATUS status, QByteArray param)
{
    qDebug() << "Current Status : " << mCurStatus << "Enter Status :" << status;

    //释放当前的资源
    if(!isworking)
    {
        isworking = true;
        if (mStProcesser[mCurStatus])
            mStProcesser[mCurStatus]->leave(param);

        //进入新的状态
        if (mStProcesser[status])
            mStProcesser[status]->enter(param);

        mCurStatus = status;
        isworking = false;
    }
}

void CMainCtrl::keyInputProc(QByteArray keyVal)
{

    if (mStProcesser[mCurStatus])
        mStProcesser[mCurStatus]->keyInputProc(keyVal);
}

void CMainCtrl::cardInputProc(QByteArray cardNo)
{
    if (mStProcesser[mCurStatus])
        mStProcesser[mCurStatus]->cardInputProc(cardNo.toHex().toUpper()); //.toHex().toUpper()
}

void CMainCtrl::coinInputProc(QByteArray coinVal)
{
    if (mStProcesser[mCurStatus])
        mStProcesser[mCurStatus]->coinInputProc(coinVal);
}

void CMainCtrl::upsInputProc(QByteArray upsVal)
{
    qDebug() << " ups : " << upsVal.toHex();

    if (mStProcesser[mCurStatus])
        mStProcesser[mCurStatus]->upsInputProc(upsVal.toHex());

}
