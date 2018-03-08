#include "takepackagepage.h"
#include "ui_takepackagepage.h"
#include "servercomm.h"
#include "driver.h"
#include "mainctrl.h"
#include "configure/db.h"
#include "lockdrv.h"
#include "exceptioncomm.h"
#include "exception.h"
#include <QDebug>
#include <QLineEdit>


//***************************************************************************
/**file		takepackagepage.cpp
* brief		取件
* author	hjzhang@163.com	2013/9/26
* version	1.3
*/
//***************************************************************************

/*
  付费方式：
     1、投币一次付清           投币钱不存数据库   支付方式  3  投币
     2、投币两次付清（间断）    不存数据库        支付方式  3   投币
     3、投币刷卡（不间断）      钱不存数据库      支付方式  4   混合
     4、投币刷卡（间断）        钱存数据库        支付方式 4   混合
     5、刷卡                  不存             支付方式  2  刷卡
     6、有已付情况的刷卡                         支付方式  4 混合


  */


#define Time_OUT         3*60*1000


takepackagepage::takepackagepage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::takepackagepage)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);

    stack = new QStackedLayout(this);

    initTakePack();

    initGetCode();

    this->setLayout(stack);

}

takepackagepage::~takepackagepage()
{
    delete ui;
}

/***************************************************************

        函数名称：initpage()
        函数功能：初始话界面
        函数参数： 无
        返回值：无

***************************************************************/

void takepackagepage::initTakePack()
{

    QWidget *takePackPage = new QWidget(this);


    takePackPage->setObjectName("QWidget");
    takePackPage->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");

//    QFont font;
//    font.setFamily(QString::fromUtf8("wenquanyi"));
//    font.setPointSize(16);

//    QPalette fontcolor;
//    fontcolor.setColor(QPalette::ButtonText,Qt::white);


    QLabel *promptLabel = new QLabel(takePackPage);
    promptLabel->resize(700,82);
    promptLabel->move(30,30);
    promptLabel->setStyleSheet("border-image: url(:/page/images/take/word1.png)");

    QLabel *testLabel = new QLabel(takePackPage);
    testLabel->resize(219,27);
    testLabel->move(95,223);
    testLabel->setStyleSheet("border-image: url(:/page/images/take/word2.png)");


    takePack_lineEdit = new CKeyBoardLineEdit(takePackPage);
    takePack_lineEdit->setMaxLength(8);
    takePack_lineEdit->resize(220,40);
    takePack_lineEdit->move(320,220);


    okBtn_takePack = new QPushButton(takePackPage);
    okBtn_takePack->resize(99,62);
    okBtn_takePack->move(560,205);
    okBtn_takePack->setFlat(true);
    okBtn_takePack->setFocusPolicy(Qt::NoFocus);
    okBtn_takePack->setStyleSheet("background-color:transparent;background-image:url(:/page/images/take/okBtn.png)");

    againBtn = new QPushButton(takePackPage);
    againBtn->setFlat(true);
    againBtn->resize(182,70);
    againBtn->move(170,494);
    againBtn->setFocusPolicy(Qt::NoFocus);
    againBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/take/getpwdBtn.png)");

    cancelBtn_takePack = new QPushButton(takePackPage);
    cancelBtn_takePack->setFlat(true);
    cancelBtn_takePack->resize(182,70);
    cancelBtn_takePack->move(453,494);
    cancelBtn_takePack->setFocusPolicy(Qt::NoFocus);
    cancelBtn_takePack->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/exitBtn.png)");

    stack->addWidget(takePackPage);


    connect(againBtn,SIGNAL(clicked()),this,SLOT(againTakeCode_slots()));
    connect(okBtn_takePack,SIGNAL(clicked()),this,SLOT(takePack_slots()));
    connect(cancelBtn_takePack,SIGNAL(clicked()),this,SLOT(quitTakePack_slots()));

    connect(takePack_lineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));

    connect(takePack_lineEdit,SIGNAL(okBtnPress()),this,SLOT(takePack_slots()));

   // connect(&debtInfoPage,SIGNAL(debtPageClose()),this,SLOT(payCoinStatus_slots()));

    connect(&takeCellInfoPage,SIGNAL(displayOpenCell()),this,SLOT(quitTakePack_slots()));

    connect(&advertisementPage,SIGNAL(advPageClose()),this,SLOT(startOpenCell()));

}


/****************************************************************
**
*	函数名称：	initGetCode
*	函数功能：	获取取件码信息
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void takepackagepage::initGetCode()
{

    QWidget *getCodePage = new QWidget();
    getCodePage->setObjectName("QWidget");
    getCodePage->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");


    QLabel *label = new QLabel(getCodePage);
    label->resize(595,41);
    label->move(30,30);
    label->setStyleSheet("border-image: url(:/page/images/take/word3.png)");

    QLabel *promptLabel = new QLabel(getCodePage);
    promptLabel->resize(135,32);
    promptLabel->move(150,223);
    promptLabel->setStyleSheet("border-image: url(:/page/images/take/word4.png)");

    getCode_lineEdit = new CKeyBoardLineEdit(getCodePage);
    getCode_lineEdit->setMaxLength(11);
    getCode_lineEdit->resize(270,40);
    getCode_lineEdit->move(330,220);

    okBtn_getCode = new QPushButton(getCodePage);
    okBtn_getCode->resize(182,70);
    okBtn_getCode->move(170,494);
    okBtn_getCode->setFlat(true);
    okBtn_getCode->setFocusPolicy(Qt::NoFocus);
    okBtn_getCode->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/okBtn.png)");


    cancelBtn_getCode = new QPushButton(getCodePage);
    cancelBtn_getCode->resize(182,70);
    cancelBtn_getCode->move(453,494);
    cancelBtn_getCode->setFlat(true);
    cancelBtn_getCode->setFocusPolicy(Qt::NoFocus);
    cancelBtn_getCode->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/cancelBtn.png)");


    stack->addWidget(getCodePage);

    progress.setMinimum(0);
    progress.setMaximum(0);
    progress.setValue(0);
    progress.setCancelButton(0);
    progress.setWindowFlags(Qt::FramelessWindowHint);
    progress.setLabelText(tr("处理中，请稍后..."));

    connect(okBtn_getCode,SIGNAL(clicked()),this,SLOT(getCode_slots()));
    connect(cancelBtn_getCode,SIGNAL(clicked()),this,SLOT(quitGetCode_slots()));

    connect(getCode_lineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));

    connect(getCode_lineEdit,SIGNAL(okBtnPress()),this,SLOT(getCode_slots()));

}


void takepackagepage::initPage()
{
    mCoinWay = false;
    mCardWay = false;
    isWorking = false;
    mFakeCoin = true;
    localWay = false;


    mDebtMoney = 0;
    mDebtCost = 0;
    mHashPay = 0;
    mOwnerCost = 0;
    mCoin = 0;
    mCellType = 0;

    mM1Card = "";

    deliveryId.clear();
    openCell.clear();

    forMySelf.clear();
    loseDate.clear();

    this->takePack_lineEdit->clear();
    this->getCode_lineEdit->clear();

    stack->setCurrentIndex(0);
}



void takepackagepage::setTakePackEnable(bool status)
{
    this->takePack_lineEdit->setReadOnly(!status);
    this->againBtn->setEnabled(status);
    this->cancelBtn_takePack->setEnabled(status);
    this->okBtn_takePack->setEnabled(status);
}


/****************************************************************
**
*	函数名称：	againTakeCode_slots
*	函数功能：	获取取件码页面
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void takepackagepage::againTakeCode_slots()
{

    emit resetTime();
    this->takePack_lineEdit->clear();
    takePack_lineEdit->setFocus();
    stack->setCurrentIndex(1);
}


/****************************************************************
**
*	函数名称：	takePack_slots
*	函数功能：	取件
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void takepackagepage::takePack_slots()
{
    emit resetTime();
    QRegExp rxpasswd("[0-9]\\d{5}");


    if(!isWorking)
    {
        isWorking = true;
        if (rxpasswd.exactMatch(this->takePack_lineEdit->text().trimmed()))
        {
            //验证码正确进入下一个取件界面(后台验证)
            //如果欠费弹欠费窗，否则告诉硬件开箱

            mCoinWay = false;
            mCardWay = false;
            mFakeCoin = true;
            localWay = false;

            mDebtCost = 0;
            mHashPay = 0;
            mCoin = 0;
            mCellType = 0;
            mBalance = 0;
            mPayMoney = 0;
            mVipPercent = 0;

            mMobilePhone.clear();
            forMySelf.clear();
            loseDate.clear();

            getExpressDebt();

        }
        else
        {
            takePack_lineEdit->clear();
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("取件码输入有误。"));          
        }
        isWorking = false;
    }

}

/****************************************************************
**
*	函数名称：	quitTakePack_slots
*	函数功能：	退出取件
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void takepackagepage::quitTakePack_slots()
{


    this->takePack_lineEdit->clear();
    this->getCode_lineEdit->clear();



    takePack_lineEdit->setFocus();
    getCode_lineEdit->setFocus();


    deliveryId.clear();
    openCell.clear();
    mDebtMoney = 0;
    mTempDebt = 0;
    mDebtCost = 0;
    mHashPay = 0;
    mOwnerCost = 0;
    mPayMoney = 0;
    mBalance = 0;
    mCoin = 0;
    mCellType = 0;


    setTakePackEnable(true);

    SCMainCtrl::instance()->enterStatus(ST_HOME, "");

}


/****************************************************************
**
*	函数名称：	getCode_slots
*	函数功能：	重新获取取件码
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void takepackagepage::getCode_slots()
{
    if(!isWorking)
    {
        isWorking = true;
        emit resetTime();
        this->okBtn_getCode->setEnabled(false);
        this->cancelBtn_getCode->setEnabled(false);

        QRegExp rxcellphone("^1\\d{10}");
        if (rxcellphone.exactMatch(this->getCode_lineEdit->text().trimmed()))
        {
            //通知后台发送短信给此手机号码        
            progress.show();

            getRanCodeAgainResp respone;
            SCServerComm::instance()->genRanCodeAgain(getCode_lineEdit->text().trimmed(),&respone);
            //加一个判断
            progress.hide();

            if(respone.success() == "true")
            {
                if(respone.messType() == "3")
                {
                    myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),tr("消息已发送，请登录手机App查看。"));
                }
                else
                {
                    myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),tr("短信已发送，请注意查收。"));
                }
               getCode_lineEdit->clear();
               stack->setCurrentIndex(0);

            }
            else if(respone.success() == RET_FALSE && respone.errCode() == DEVICE_ERROR)
            {
                SCMainCtrl::instance()->enterStatus(ST_DEVICE_REST,"");
            }
            else if(respone.success() == "false" && respone.errCode() == "10004")
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("您取短信的次数已超上限，详情请咨询客服。"));
            }
            else if(respone.success() == "false")
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("系统没有您的派件。"));
            }
            else
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"));
            }


        }
        else
        {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("手机号码输入有误,请输入正确号码。"));

        }

        this->okBtn_getCode->setEnabled(true);
        this->cancelBtn_getCode->setEnabled(true);
        isWorking = false;
    }
}



void takepackagepage::quitGetCode_slots()
{
    emit resetTime();
    this->getCode_lineEdit->clear();
    this->getCode_lineEdit->setFocus();
    stack->setCurrentIndex(0);
}





/****************************************************************
**
*	函数名称：	getExpressDebt
*	函数功能：	获取欠费数额
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void takepackagepage::getExpressDebt()
{
    emit resetTime();
    this->okBtn_takePack->setEnabled(false);
    this->cancelBtn_takePack->setEnabled(false);
    this->againBtn->setEnabled(false);
    this->takePack_lineEdit->setReadOnly(true);


    QString randomCode = takePack_lineEdit->text().trimmed();
    getDeliveryByCodeResp respone;

    progress.show();

    SCServerComm::instance()->getDeliveryByCode(randomCode,&respone);

    progress.hide();

    takePack_lineEdit->clear();
    if (respone.success() == "true")
    {
        openCell = respone.cellId();

        mMobilePhone = respone.mobilePhone();
        mCellType = respone.scellType().toInt();              //箱子类型 大中小
        mDebtCost = respone.debt().toDouble();               //滞留费
        mOwnerCost = respone.delOwnerCost().toDouble();      //个人需付费
        mHashPay = respone.ownerHasPay().toDouble();         //已付金额
        mVipPercent = respone.vipPercent().toDouble();       //打折率
        mDebtMoney = QString::number(mDebtCost + mOwnerCost - mHashPay,'f',2).toDouble();   //获取欠款的钱
        mTempDebt = mDebtMoney;

        deliveryId = respone.deliveryId();

        if(mHashPay > 0)
        {
            mCoinWay = true;
        }

        int retValue = againCheckCellProc();

        if(retValue == 1)
        {
            if(mDebtMoney <= 0)
            {

                advertisementPage.advShowPage(deliveryId);
                //advertisementPage.exec();

                /*
                lockDrv.openLock(cabinetId,cellId);
               // SCDriver::instance()->openCellLock(cabinetId,cellId);  //开锁  默认成功
                reportServer();
                                          *///报告服务器派件取走
            }
            else
            {
                debtInfoPage.setCoinLab(QString("%1").arg(mDebtMoney));
                debtInfoPage.debtTimeStart(deliveryId,respone.vipPercent(),mDebtCost,mOwnerCost,mHashPay);
            }
        }
        else if(retValue == 0)
        {
           myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("此终端没有您的派件"));
        }
        else
        {
           myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("箱门打不开,请等待客服处理，处理完毕将会短信通知。"));
        }

    }
    else if(respone.success() == RET_FALSE && respone.errCode() == DEVICE_ERROR)
    {
        SCMainCtrl::instance()->enterStatus(ST_DEVICE_REST,"");
    }
    else if(respone.success() == "false" && respone.errCode() == "10001")
    {
        myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("箱门打不开,请等待客服处理，处理完毕将会短信通知。"));
    }
    else if(respone.success() == "false")
    {
        myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("您输入的取件码不正确！"));
    }
    else
    {
        localTakePackage(randomCode);
       // myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"));
    }

    this->okBtn_takePack->setEnabled(true);
    this->cancelBtn_takePack->setEnabled(true);
    this->againBtn->setEnabled(true);
    this->takePack_lineEdit->setReadOnly(false);


}


/****************************************************************
**
*	函数名称：	localTakePackage
*	函数功能：	断网本地取件
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/

void takepackagepage::localTakePackage(QString randomCode)
{
    TakeInfoDesc_t takeInfo;
    if(SCDatBase::instance()->queryTakePackageInfo(takeInfo,randomCode))
    {
        openCell = takeInfo.opencell;

        mCellType = takeInfo.celltype.toInt();              //箱子类型 大中小
        mDebtCost = 0;                              //滞留费
        mOwnerCost = 0;                             //个人需付费
        mHashPay = 0;                               //已付金额
        mVipPercent = 0;                            //打折率
        mDebtMoney = 0;                             //获取欠款的钱
        mTempDebt = mDebtMoney;

        deliveryId = takeInfo.delivery;

        int retValue = againLocalCheckCellProc();

        if(retValue == 1)
        {
             localWay = true;
             advertisementPage.advShowPage(deliveryId);
        }
        else if(retValue == 0)
        {
           myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("此终端没有您的派件"));
        }
        else
        {
           myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("箱门打不开,请等待客服处理，处理完毕将会短信通知。"));
        }

    }
    else
    {
        myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"));
    }
}


/****************************************************************
**
*	函数名称：	againLocalCheckCellProc
*	函数功能：	检测箱门是否打不开
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/

int takepackagepage::againLocalCheckCellProc()
{
    QStringList list;
    QList<ServerDesc_t> serverlist;

    SCDatBase::instance()->queryServerException(serverlist);

    for(int i = 0;i < serverlist.size(); i++)
    {
        if(serverlist.at(i).errType == "3")    //取件箱门打不开类型
        {
            list = serverlist.at(i).cellId.split("|"); //为刷卡取件有多个箱门打不开服务 XX|XX
            for(int j = 0; j < list.size(); j++)
            {
                if(list.at(j) == openCell)
                {
                    return -1;
                }
            }
        }


        if(serverlist.at(i).errType == "2")   //件已被用户取走
        {
            if(serverlist.at(i).cellId == deliveryId)
            {
                return 0;
            }
        }

        if(serverlist.at(i).errType == "5")   //件已被快递员取走
        {
            list = serverlist.at(i).cellId.split("|"); //件被快递员取走 XX|XX
            for(int j = 0; j < list.size(); j++)
            {
                if(list.at(j) == deliveryId)
                {
                    return 0;
                }
            }
        }
    }

    return 1;
}



/****************************************************************
**
*	函数名称：	againCheckCellProc
*	函数功能：	检测箱门是否打不开，欠费过滤
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/

int takepackagepage::againCheckCellProc()
{
    QStringList list;
    QList<ServerDesc_t> serverlist;
    QList<DebtDesc_t> debtlist;
    SCDatBase::instance()->queryDebtException(debtlist);
    SCDatBase::instance()->queryServerException(serverlist);

    for(int i = 0;i < serverlist.size(); i++)
    {
        if(serverlist.at(i).errType == "3")    //取件箱门打不开类型
        {
            list = serverlist.at(i).cellId.split("|"); //为刷卡取件有多个箱门打不开服务 XX|XX
            for(int j = 0; j < list.size(); j++)
            {
                if(list.at(j) == openCell)
                {
                    return -1;
                }
            }
        }


        if(serverlist.at(i).errType == "2")   //件已被用户取走
        {
            if(serverlist.at(i).cellId == deliveryId)
            {
                return 0;
            }
        }

        if(serverlist.at(i).errType == "5")   //件已被快递员取走
        {
            list = serverlist.at(i).cellId.split("|"); //件被快递员取走 XX|XX
            for(int j = 0; j < list.size(); j++)
            {
                if(list.at(j) == deliveryId)
                {
                    return 0;
                }
            }
        }
    }

    for(int k = 0;k < debtlist.size();k++)   //欠费钱
    {
        if(debtlist.at(k).deliveryId == deliveryId)
        {
            if(mDebtMoney > 0)
            {
                mCoinWay = true;
                mDebtMoney = mDebtMoney - debtlist.at(k).debt;
                mHashPay = mHashPay + debtlist.at(k).debt;
                mTempDebt = mDebtMoney;
            }
            break;
        }
    }

    if(mDebtMoney > 0)
    {
        QDateTime coinTime = QDateTime::fromString(SCMainCtrl::instance()->expiryCoin.keys().at(0),Qt::ISODate);
        qint8 tempCoins = SCMainCtrl::instance()->expiryCoin.values().at(0);
        bool status = true;
        qint8 coins;

        if((coinTime.secsTo(QDateTime::currentDateTime()) < COIN_EXPIRY) && (tempCoins > 0))
        {
            if(tempCoins >= mDebtMoney)
            {
                mFakeCoin = false; //当箱门打不开时不要计数
                coins = mDebtMoney;
                SCMainCtrl::instance()->expiryCoin.clear();
                SCMainCtrl::instance()->expiryCoin.insert(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),tempCoins-mDebtMoney);
            }
            else
            {
                coins = tempCoins;
                SCMainCtrl::instance()->expiryCoin.clear();
                SCMainCtrl::instance()->expiryCoin.insert(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),0);
            }

            //缓存的硬币划分给某个具体的派件
            for(int j = 0;j < debtlist.size();j++)
            {
                if(debtlist.at(j).deliveryId == deliveryId)
                {
                    qint8 debt = debtlist.at(j).debt + coins;
                    SCDatBase::instance()->setDebtException(debtlist.at(j).index,debt);
                    status = false;
                    break;
                }
            }

            if(status)
            {
                SCDatBase::instance()->insertDebtException(deliveryId,coins);
            }

            mCoinWay = true;
            mDebtMoney = mDebtMoney - tempCoins;
            mHashPay = mHashPay + tempCoins;
            mTempDebt = mDebtMoney;
        }
    }

    return 1;
}

/****************************************************************
**
*	函数名称：	paycoin_slot
*	函数功能：	投币触发的槽
* 	函数参数：	   无
* 	返回值  :       无
*
****************************************************************/
void takepackagepage::payCoin()
{

    if(debtInfoPage.isVisible())
    {
        mTempDebt--;
        mCoin++;
        mCoinWay = true;
        debtInfoPage.setCoinLab(QString("%1").arg(mTempDebt));
        if(mTempDebt <= 0)
        {
            debtInfoPage.closeCurDebtPage();
            advertisementPage.advShowPage(deliveryId);

        }
    }
    else
    {
        //投币缓存5分钟清空一次
        qint8 tempExpiryCoins = SCMainCtrl::instance()->expiryCoin.values().at(0);
        QDateTime coinTime = QDateTime::fromString(SCMainCtrl::instance()->expiryCoin.keys().at(0),Qt::ISODate);

        if(coinTime.secsTo(QDateTime::currentDateTime()) < COIN_EXPIRY)
        {
            tempExpiryCoins++;
            SCMainCtrl::instance()->expiryCoin.clear();
            SCMainCtrl::instance()->expiryCoin.insert(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),tempExpiryCoins);

        }
        else
        {
            tempExpiryCoins = 1;
            SCMainCtrl::instance()->expiryCoin.clear();
            SCMainCtrl::instance()->expiryCoin.insert(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),tempExpiryCoins);

        }
    }
}



/****************************************************************
**
*	函数名称：	paycoin_card_slot
*	函数功能：	刷卡触发的槽
* 	函数参数：	   无
* 	返回值  :       无
*
****************************************************************/
void takepackagepage::payCard(QString m1Card)
{

    //刷卡交互返回信息
    mM1Card = m1Card;
    if(debtInfoPage.isVisible())
    {
        progress.show();
        debtInfoPage.setBtnEnable(false);
        getVIPCardInfoResp respone;
        SCServerComm::instance()->getVIPCardInfo(deliveryId,m1Card,&respone);
        debtInfoPage.setBtnEnable(true);
        progress.hide();

        if(respone.success() == RET_FALSE && respone.errCode() == IC_FORBID_ERROR)
        {
            debtInfoPage.promptInfo(QMessageBox::Warning,tr("提示"),tr("您的会员卡已被冻结，详情请咨询客服。"),Qt::NonModal);

        }
//        else if(respone.success() == RET_FALSE && respone.errCode() == IC_DATE_ERROR)
//        {
//            debtInfoPage.promptInfo(QMessageBox::Warning,tr("提示"),tr("会员卡已过有效期。"),Qt::NonModal);
//        }
//        else if(respone.success() == RET_FALSE && respone.errCode() == IC_PHONE_ERROR)
//        {
//            debtInfoPage.promptInfo(QMessageBox::Warning,tr("提示"),tr("会员卡注册信息与派件手机号不符。"),Qt::NonModal);
//        }
        else if(respone.success() == RET_FALSE && respone.errCode() == NOTEXIST_IC_ERROR)
        {
            debtInfoPage.promptInfo(QMessageBox::Warning,tr("提示"),tr("卡号不存在。"),Qt::NonModal);
        }
        else if(respone.success() == RET_FALSE && respone.errCode() == recharge_IC_ERROR)
        {
            debtInfoPage.promptInfo(QMessageBox::Warning,tr("提示"),tr("帐户不存在。"),Qt::NonModal);

        }
        else if(respone.success() == RET_TRUE)
        {
//            mTempDebt = mTempDebt - mOwnerCost;
//            mOwnerCost = 0;
//            debtInfoPage.updateCoinLab(QString("%1").arg(mTempDebt));
//            mCardWay = true;
//            QString phone = respone.mobilePhone();
//            double balance = respone.balance().toDouble();
//            double vipgrade = respone.vipGrade().toDouble();
//            double vipPercent = respone.vipPercent().toDouble();
//            double payMoney = QString::number((mDebtMoney - mCoin)*vipPercent,'f',2).toDouble();
//            mBalance = balance - payMoney;
//            mPayMoney = payMoney;

//              if(mTempDebt <= 0)
//              {
//                 debtInfoPage.closeCurDebtPage();

//                 advertisementPage.advShowPage(deliveryId);
//                 advertisementPage.exec();
//              }


            handleCardInfo(&respone);
        }
        else
        {
            debtInfoPage.promptInfo(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"),Qt::NonModal);
        }

    }
    else if(stack->currentIndex() == 0 && advertisementPage.isHidden())
    {
        progress.show();
        getDelsByVIPCardResp ret;
        QByteArray retAry;
        setTakePackEnable(false);
        retAry = SCServerComm::instance()->getDelsByVIPCard(mM1Card,&ret);
        setTakePackEnable(true);

        progress.hide();
        if(ret.success() == RET_FALSE)
        {
            if(ret.errCode() == IC_FORBID_ERROR)
            {
                debtInfoPage.promptInfo(QMessageBox::Warning,tr("提示"),tr("您的会员卡已被冻结，详情请咨询客服。"),Qt::NonModal);
            }
            else if(ret.errCode() == CARD_NO_PACKAGE)
            {
                debtInfoPage.promptInfo(QMessageBox::Warning,tr("提示"),tr("该终端没有您的包裹。"),Qt::NonModal);
            }
            else if(ret.errCode() == NOTEXIST_IC_ERROR)
            {
                debtInfoPage.promptInfo(QMessageBox::Warning,tr("提示"),tr("卡号不存在。"),Qt::NonModal);
            }
            else if(ret.errCode() == recharge_IC_ERROR )
            {
                debtInfoPage.promptInfo(QMessageBox::Warning,tr("提示"),tr("帐户不存在。"),Qt::NonModal);
            }
        }
        else if(ret.success() == RET_TRUE)
        {
             SCMainCtrl::instance()->enterStatus(ST_GET_CARD,retAry);
        }
        else
        {
             debtInfoPage.promptInfo(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"),Qt::NonModal);
        }

    }

}

/****************************************************************
**
*	函数名称：	handleCardInfo
*	函数功能：	处理用户刷卡付费信息
* 	函数参数：	getVIPCardInfoResp
* 	返回值  :       无
*
****************************************************************/
void takepackagepage::handleCardInfo(getVIPCardInfoResp *respone)
{
    loseDate = respone->isExpired();
    QString phone = respone->mobilePhone();
    double balance = respone->balance().toDouble();
    double vipgrade = respone->vipGrade().toDouble();
    double vipPercent = respone->vipPercent().toDouble();

    double pay;//当前需付滞留费在有效期内
    double overPay;//需付钱不再有效期内
    double payMoney = 0;//刷卡应付

    mBalance = balance;
    //包裹信息与刷卡信息手机号对等
    if(phone == mMobilePhone)
    {
        forMySelf = "yes";
        pay = mDebtMoney - mCoin - mOwnerCost; //当前需付滞留费在有效期内
        overPay = mDebtMoney - mCoin;          //需付钱不再有效期内
        if("no" == loseDate)
        {
            if(pay > 0)
            {
                payMoney = QString::number(pay*mVipPercent,'f',2).toDouble();//刷卡应付
                if(payMoney > balance)
                {
                    debtInfoPage.promptInfo(QMessageBox::Warning,tr("提示"),tr("余额不足，请投币或充值付滞留费用。"),Qt::NonModal);
                }
                else
                {
                    mBalance = balance - payMoney;  //余额
                    mPayMoney = payMoney;           //付费

                    mCardWay = true;
                    debtInfoPage.closeCurDebtPage();

                    advertisementPage.advShowPage(deliveryId);
                   // advertisementPage.exec();
                }
            }
            else
            {
                mCardWay = true;
                debtInfoPage.closeCurDebtPage();
                advertisementPage.advShowPage(deliveryId);
                //advertisementPage.exec();
            }
        }
        else if("yes" == loseDate)
        {
            if(overPay > 0)
            {
                payMoney = QString::number(overPay*mVipPercent,'f',2).toDouble();//刷卡应付
                if(payMoney > balance)
                {
                    debtInfoPage.promptInfo(QMessageBox::Warning,tr("提示"),tr("套餐已过期，余额不足，请投币或充值。"),Qt::NonModal);
                }
                else
                {
                    mBalance = balance - payMoney;  //余额
                    mPayMoney = payMoney;           //付费

                    mCardWay = true;
                    debtInfoPage.closeCurDebtPage();
                    advertisementPage.advShowPage(deliveryId);
                    //advertisementPage.exec();
                }
            }
            else
            {
                mCardWay = true;
                debtInfoPage.closeCurDebtPage();
                advertisementPage.advShowPage(deliveryId);
               // advertisementPage.exec();
            }
        }
    }

    //包裹信息与刷卡信息手机号不对等
    else
    {
        forMySelf = "no";
        overPay = mDebtMoney - mCoin;          //需付钱不再有效期内
        if(overPay > 0)
        {
            payMoney = QString::number(overPay*mVipPercent,'f',2).toDouble();//刷卡应付
            if(payMoney > balance)
            {
                debtInfoPage.promptInfo(QMessageBox::Warning,tr("提示"),tr("余额不足，请投币或充值。"),Qt::NonModal);
            }
            else
            {
                mBalance = balance - payMoney;  //余额
                mPayMoney = payMoney;           //付费

                mCardWay = true;
                debtInfoPage.closeCurDebtPage();
                advertisementPage.advShowPage(deliveryId);
                //advertisementPage.exec();
            }
        }
        else
        {
            mCardWay = true;
            debtInfoPage.closeCurDebtPage();
            advertisementPage.advShowPage(deliveryId);
           // advertisementPage.exec();
        }
    }
}


/****************************************************************
**
*	函数名称：	startOpenCell
*	函数功能：	播放广告准备开始打开箱门信息
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void takepackagepage::startOpenCell()
{
    qint8 cabinetId;
    qint8 cellId;
    bool ok;

    cabinetId = openCell.mid(openCell.size() - 4,2).toInt(&ok,10);

    cellId = openCell.mid(openCell.size() - 2,2).toInt(&ok,10);

    openBoxLock(cabinetId,cellId);

    advertisementPage.close();
}

/****************************************************************
**
*	函数名称：	openBoxLock
*	函数功能：	报告后台信息
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void takepackagepage::openBoxLock(qint8 cabinetId, qint8 cellId)
{

    if(lockoperation.openLock(cabinetId,cellId) == 1)
    {
        QString tempErrType = "2";

        //上报打开
        reportServer();

        //插入打开的箱门到数据库
        SCDatBase::instance()->insertOpenException(cabinetId,cellId,mCellType,tempErrType);

        //等待检测的箱门
      //  SExceptionThrd::instance()->setWaitQueryCell(openCell);
        //开一个定时器3分钟扫描箱门是否关闭
    //   QTimer::singleShot(Time_OUT, this, SLOT(timeOver()));

        //播放音乐
        QString mp3file = "./music/take.mp3";
        QString vol = "15";
        SCDriver::instance()->playVoice(mp3file,vol);

        if(mCardWay && mPayMoney > 0)
        {
            takeCellInfoPage.setCellLab(openCell,mPayMoney,mBalance);
        }
        else
        {
            takeCellInfoPage.setCellLab(openCell);
        }

    }
    else
    {
        QList<cellErrorInfo> boxExceptionList;
        cellErrorInfo notOpenInfo;

        //取件判断是否打不开


        int length = openCell.size();

        notOpenInfo.scabId = openCell.mid(0,length - 2);
        notOpenInfo.scellId = openCell;
        notOpenInfo.errType = "3";
        notOpenInfo.relDeliveryId = deliveryId;
        boxExceptionList.append(notOpenInfo);

        reportCellErrorsResp respone;
        SCExceptioncomm::instance()->reportCellErrors(boxExceptionList,&respone);



        if(respone.success() == "false")
        {
            qDebug() << "happen exception error";
        }

        notOpenRecordCoin();
        myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("箱门打不开,请等待客服处理，处理完毕将会短信通知。"));

      }

}


//打不开记录最终硬币个数同时更新数据库中的数据

void takepackagepage::notOpenRecordCoin()
{
    if(mCoin > 0 && mFakeCoin)
    {
        bool status = true;
        QList<DebtDesc_t> debtlist;
        SCDatBase::instance()->queryDebtException(debtlist);
        for(int i = 0;i < debtlist.size();i++)
        {
            if(debtlist.at(i).deliveryId == deliveryId)
            {
                qint8 debt = debtlist.at(i).debt + mCoin;
                SCDatBase::instance()->setDebtException(debtlist.at(i).index,debt);
                status = false;
                break;
            }
        }

        if(status)
        {
            SCDatBase::instance()->insertDebtException(deliveryId,mCoin);
        }
    }
}


/****************************************************************
**
*	函数名称：	reportServer
*	函数功能：	报告后台信息
* 	函数参数：	无
* 	返回值  :    无
*
****************************************************************/
void takepackagepage::reportServer()
{
    CellInfoDesc_t cellInfo;
    SCDatBase::instance()->getBoxCount(cellInfo);
    getDeliveryConfirmResp respone;
    QString accountPaid = QString::number(mPayMoney + mCoin,'f',2);//QString::number(mDebtMoney,'f',2);
    QString debtCost = QString::number(mDebtCost,'f',2);
    QString payWay;
    QString cashPayMoney = QString::number(mCoin,'f',2);

    if((mCoinWay && mCardWay)||(mCardWay && mHashPay > 0))
    {
        payWay = "4";
    }
    else if(mCoinWay)
    {
        payWay = "3";
    }
    else if(mCardWay)
    {
        payWay = "2";
    }
    else if(localWay)
    {
        payWay = "5";
    }
    else
    {
        payWay = "1";
    }

    //删除本地存储信息
    SCDatBase::instance()->deleteTakePackageInfo(deliveryId);
    SCServerComm::instance()->getDeliveryConfirm(deliveryId,accountPaid,debtCost,payWay,cashPayMoney,mM1Card,forMySelf,loseDate,&respone);

    if(respone.success() == "true")
    {
//        if(respone.scellType() == "1")
//        {
//            SCDatBase::instance()->setBoxCount(cellInfo.bigCell + 1,cellInfo.middleCell,cellInfo.smallCell);
//        }
//        else if(respone.scellType() == "2")
//        {
//            SCDatBase::instance()->setBoxCount(cellInfo.bigCell,cellInfo.middleCell + 1,cellInfo.smallCell);
//        }
//        else
//        {
//            SCDatBase::instance()->setBoxCount(cellInfo.bigCell,cellInfo.middleCell,cellInfo.smallCell + 1);
//        }

//        byteArry.append("REFRESHCELL");  //可以更新箱门数量
    }
    else
    {
        // qDebug() << "takepackagepage is error" << respone.errMessage();
    }

}




void takepackagepage::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
