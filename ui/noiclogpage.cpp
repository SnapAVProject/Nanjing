#include "noiclogpage.h"
#include <QLabel>
#include <QPushButton>
#include "servercomm.h"
#include "mainctrl.h"
#include "configure/db.h"
#include "QDebug"

noiclogpage::noiclogpage()
{
    stack = new QStackedLayout(this);

    initM1Code();
    initNoM1Log();

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800,600);

    this->setLayout(stack);
}

noiclogpage::~noiclogpage()
{

}


void noiclogpage::initM1Code()
{
    QWidget *logSavePage = new QWidget(this);
    logSavePage->setObjectName("QWidget");
    logSavePage->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");


    QLabel *ic_prompt = new QLabel(logSavePage);
    ic_prompt->resize(330,42);
    ic_prompt->move(10,30);
    ic_prompt->setStyleSheet("border-image: url(:/page/images/noIc/word1.png)");


    QLabel *take_lab = new QLabel(logSavePage);
    take_lab->resize(108,32);
    take_lab->move(150,220);
    take_lab->setAlignment(Qt::AlignCenter);
    take_lab->setStyleSheet("border-image: url(:/page/images/noIc/word2.png)");

    take_edit = new CKeyBoardLineEdit(logSavePage);
    take_edit->setMaxLength(8);
    take_edit->resize(250,40);
    take_edit->move(260,220);
    take_edit->setEchoMode(QLineEdit::Password);


    okBtn_logSave = new QPushButton(logSavePage);
    okBtn_logSave->resize(99,62);
    okBtn_logSave->move(530,205);
    okBtn_logSave->setFlat(true);
    okBtn_logSave->setFocusPolicy(Qt::NoFocus);
    okBtn_logSave->setStyleSheet("background-color:transparent;background-image:url(:/page/images/noIc/loginBtn.png)");

    registerBtn = new QPushButton(logSavePage);
    registerBtn->setFlat(true);
    registerBtn->resize(182,70);
    registerBtn->move(170,494);
    registerBtn->setFocusPolicy(Qt::NoFocus);
    registerBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/noIc/getpwdBtn.png)");

    cancelBtn_logSave = new QPushButton(logSavePage);
    cancelBtn_logSave->setFlat(true);
    cancelBtn_logSave->resize(182,70);
    cancelBtn_logSave->move(453,494);
    cancelBtn_logSave->setFocusPolicy(Qt::NoFocus);
    cancelBtn_logSave->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/exitBtn.png)");


    stack->addWidget(logSavePage);

    connect(okBtn_logSave,SIGNAL(clicked()),this,SLOT(logSavePage_slot()));
    connect(registerBtn,SIGNAL(clicked()),this,SLOT(register_slot()));
    connect(cancelBtn_logSave,SIGNAL(clicked()),this,SLOT(cancelLogSavePage_slot()));

    connect(take_edit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));
    connect(take_edit,SIGNAL(okBtnPress()),this,SLOT(logSavePage_slot()));
    connect(&myBox,SIGNAL(message_ok()),this,SLOT(takeDelayPackage()));

}

void noiclogpage::initNoM1Log()
{
    QWidget *getInfoPage = new QWidget(this);

    getInfoPage->setObjectName("QWidget");
    getInfoPage->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");

    QLabel *ic_prompt = new QLabel(getInfoPage);
    ic_prompt->resize(608,41);
    ic_prompt->move(10,30);
    ic_prompt->setStyleSheet("border-image: url(:/page/images/noIc/word3.png)");


    QLabel *phone_lab = new QLabel(getInfoPage);
    phone_lab->resize(132,34);
    phone_lab->move(140,220);
    phone_lab->setStyleSheet("background-image: url(:/page/images/noIc/word4.png)");

    phone_edit = new CKeyBoardLineEdit(getInfoPage);
    phone_edit->setMaxLength(11);
    phone_edit->resize(280,40);
    phone_edit->move(350,220);


    okBtn_getSaveCode = new QPushButton(getInfoPage);
    okBtn_getSaveCode->setFlat(true);
    okBtn_getSaveCode->resize(182,70);
    okBtn_getSaveCode->move(170,494);
    okBtn_getSaveCode->setFocusPolicy(Qt::NoFocus);
    okBtn_getSaveCode->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/okBtn.png)");

    cancelBtn_getSaveCode = new QPushButton(getInfoPage);
    cancelBtn_getSaveCode->setFlat(true);
    cancelBtn_getSaveCode->resize(182,70);
    cancelBtn_getSaveCode->move(453,494);
    cancelBtn_getSaveCode->setFocusPolicy(Qt::NoFocus);
    cancelBtn_getSaveCode->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/cancelBtn.png)");

    stack->addWidget(getInfoPage);

    connect(okBtn_getSaveCode,SIGNAL(clicked()),this,SLOT(getSaveCode_slot()));
    connect(cancelBtn_getSaveCode,SIGNAL(clicked()),this,SLOT(cancelSaveCode_slot()));

    connect(phone_edit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));
    connect(phone_edit,SIGNAL(okBtnPress()),this,SLOT(getSaveCode_slot()));

}

void noiclogpage::register_slot()
{
    emit resetTime();
    this->take_edit->clear();
    this->phone_edit->clear();
    this->phone_edit->setFocus();
    stack->setCurrentIndex(1);
}


/****************************************************************
**
*	函数名称：	getSaveCode_slot
*	函数功能：	获取存件码信息
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void noiclogpage::getSaveCode_slot()
{
    emit resetTime();

    if(!isWorking)
    {
        isWorking = true;
        this->okBtn_getSaveCode->setEnabled(false);
        this->cancelBtn_getSaveCode->setEnabled(false);

        //手机号发给后台
        QRegExp rxcellphone("^1\\d{10}");
        getRanLoginCodeResp respone;
        if(rxcellphone.exactMatch(this->phone_edit->text().trimmed()))
        {
            progress.setMinimum(0);
            progress.setMaximum(0);
            progress.setValue(0);
            progress.setCancelButton(0);
            progress.setWindowFlags(Qt::FramelessWindowHint);
            progress.setLabelText(tr("处理中，请稍后..."));
            progress.show();

            SCServerComm::instance()->genRanLoginCode(this->phone_edit->text().trimmed(),&respone);
            progress.hide();
            if(respone.success() == "false" && respone.errCode() == "10004")
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("您取短信的次数已超上限，详情请咨询客服。"));
            }
            else if(respone.success() == RET_FALSE && respone.errCode() == DEVICE_ERROR)
            {
                SCMainCtrl::instance()->enterStatus(ST_DEVICE_REST,"");
            }
            else if(respone.success() == "false" && respone.errCode() == IC_FORBID_ERROR)
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("您的会员卡已被冻结，详情请咨询客服。"));
            }
            else if(respone.success() == "false")
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("此号码未登记，请输入正确的会员卡手机号码。"));
            }
            else if(respone.success() == "true")
            {
                if(respone.messType() == "3")
                {
                    myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),tr("消息已发送，请登录手机App查看。"));
                }
                else
                {
                    myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),tr("短信已发送，请注意查收"));
                }
                phone_edit->clear();
                stack->setCurrentIndex(0);
            }
            else
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"));
            }
        }
        else
        {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("手机号码输入有误！请输入正确号码。"));
        }

        this->okBtn_getSaveCode->setEnabled(true);
        this->cancelBtn_getSaveCode->setEnabled(true);
        isWorking = false;
    }

}



void noiclogpage::takeDelayPackage()
{
    QByteArray ret;
    deliveryTimeoutResp respone;

    ret = SCServerComm::instance()->deliveryTimeout(mM1Card,mMobilePhone,&respone);
    if(respone.errCode() == "0000" && respone.getDeliveryInfoList().size() > 0)
    {
        SCMainCtrl::instance()->enterStatus(ST_COURIER_TAKE,ret);
    }
    else if(respone.errCode() == "0000")
    {
         myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),tr("您没有滞留件需要处理。"));
    }
    else if(respone.errCode() == "10009")
    {
         myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),tr("卡号不存在。"));
    }
}


void noiclogpage::cancelSaveCode_slot()
{
    emit resetTime();
    this->take_edit->clear();
    this->take_edit->setFocus();
    stack->setCurrentIndex(0);
}


void noiclogpage::startOpenCellNoIcLog()
{
    okBtn_logSave->setEnabled(false);
    cancelBtn_logSave->setEnabled(false);
    this->registerBtn->setEnabled(false);

    checkOpenScellResp respone;

    progress.setMinimum(0);
    progress.setMaximum(0);
    progress.setValue(0);
    progress.setCancelButton(0);
    progress.setWindowFlags(Qt::FramelessWindowHint);
    progress.setLabelText(tr("处理中，请稍后..."));
    progress.show();

    SCServerComm::instance()->noCardCheckOpenScell(this->take_edit->text().trimmed(),&respone);
    progress.hide();
    take_edit->clear();

    okBtn_logSave->setEnabled(true);
    cancelBtn_logSave->setEnabled(true);
    this->registerBtn->setEnabled(true);

    if(respone.errCode() == "0000") //卡类型符合空箱处理
    {
        SCMainCtrl::instance()->enterStatus(ST_EXCEPTION_OPENDOOR,"");
    }
    else if(!respone.errCode().isEmpty())
    {
         myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),respone.errMessage());
    }
    else
    {
        myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),tr("网络异常,请稍后再试。"));
    }



}


void noiclogpage::startDetainNoIcLog()
{
    okBtn_logSave->setEnabled(false);
    cancelBtn_logSave->setEnabled(false);
    this->registerBtn->setEnabled(false);

    QByteArray ret;
    deliveryTimeoutResp respone;

    progress.setMinimum(0);
    progress.setMaximum(0);
    progress.setValue(0);
    progress.setCancelButton(0);
    progress.setWindowFlags(Qt::FramelessWindowHint);
    progress.setLabelText(tr("处理中，请稍后..."));
    progress.show();

    ret = SCServerComm::instance()->noCardDeliveryTimeout(this->take_edit->text().trimmed(),&respone);
    progress.hide();
    take_edit->clear();

    okBtn_logSave->setEnabled(true);
    cancelBtn_logSave->setEnabled(true);
    this->registerBtn->setEnabled(true);

    if(respone.errCode() == "0000" && respone.getDeliveryInfoList().size() > 0)
    {
        SCMainCtrl::instance()->enterStatus(ST_COURIER_TAKE,ret);
    }
    else if(respone.errCode() == "0000")
    {
         myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),tr("您没有滞留件需要处理。"));
    }
    else if(!respone.errCode().isEmpty())
    {
         myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),respone.errMessage());
    }
    else
    {
        myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),tr("网络异常,请稍后再试。"));
    }

}



void noiclogpage::startCourierTakeNoIcLog()
{
    okBtn_logSave->setEnabled(false);
    cancelBtn_logSave->setEnabled(false);
    this->registerBtn->setEnabled(false);
    QByteArray ret;
    expGetSaveDeliveryResp respone;
    QString data;

    progress.setMinimum(0);
    progress.setMaximum(0);
    progress.setValue(0);
    progress.setCancelButton(0);
    progress.setWindowFlags(Qt::FramelessWindowHint);
    progress.setLabelText(tr("处理中，请稍后..."));
    progress.show();

    ret = SCServerComm::instance()->expGetSaveDelivery(data,this->take_edit->text().trimmed(),&respone);

    progress.hide();
    okBtn_logSave->setEnabled(true);
    cancelBtn_logSave->setEnabled(true);
    this->registerBtn->setEnabled(true);

    if(respone.errCode() == "0000" && respone.getDeliveryInfoList().size() > 0)
    {
        SCMainCtrl::instance()->enterStatus(ST_COURIER_HELP_TAKE,ret);
    }
    else if(respone.errCode() == "0000")
    {
         myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),tr("该终端没有您存的快件。"));
    }
    else if(!respone.errCode().isEmpty())
    {
         myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),respone.errMessage());
    }
    else
    {
        myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),tr("网络异常,请稍后再试。"));
    }
}



void noiclogpage::startNoIcLog()
{
     okBtn_logSave->setEnabled(false);
     cancelBtn_logSave->setEnabled(false);
     this->registerBtn->setEnabled(false);

     m1CardLoginResp respone ;

     progress.setMinimum(0);
     progress.setMaximum(0);
     progress.setValue(0);
     progress.setCancelButton(0);
     progress.setWindowFlags(Qt::FramelessWindowHint);
     progress.setLabelText(tr("处理中，请稍后..."));
     progress.show();

     SCServerComm::instance()->noM1CardLogin(this->take_edit->text().trimmed(),&respone);
     progress.hide();
     take_edit->clear();

     okBtn_logSave->setEnabled(true);
     cancelBtn_logSave->setEnabled(true);
     this->registerBtn->setEnabled(true);

     if(respone.success() == RET_TRUE && respone.errCode() == OVERDUE_PACKAGE)
     {
        mM1Card = respone.m1CardId();
        mMobilePhone = respone.mobilePhone();
        myBox.myPrompt(QMessageBox::Warning,tr("提示确认"),tr("您存在滞留快件，取出后才可继续存件，是否取出？"));
     }
     else if(respone.success() == RET_TRUE && ((respone.cardType() == "2") || (respone.cardType() == "4")))
     {
         QString personType;
         if(respone.cardType() == "2")
         {
             personType = "KD";
         }
         else
         {
            personType = "SC"; //蔬菜快递员
         }
         bool isFix = false;
         if( !respone.orderBigCells().isEmpty() || !respone.orderMiddleCells().isEmpty() || !respone.orderSmallCells().isEmpty() || !respone.orderVegeCells().isEmpty())
         {
             isFix = true;
         }

         if(respone.getArrayNew().size()!= 0)     //无确认存件
         {
             SCMainCtrl::instance()->enterStatus(ST_PAKEAGE_CONFIRM,waitconfirmByteArray(respone,personType,isFix));  //存件确认
         }
         else
         {
             if((respone.tariffType().toInt() == 2)||(respone.tariffType().toInt() == 4))
             {
                 double balance = respone.balance().toDouble();
                 if((!isFix && balance <= 0) || (isFix && balance < 0))
                 {
                     myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("您的账户余额不足，请先充值。"));
                     return;
                 }
             }

             QByteArray     outData;
             QDataStream out(&outData,QIODevice::WriteOnly);
             out << respone.m1CardId() << personType << respone.bigCellIdList();
             out << respone.middleCellIdList() << respone.smallCellIdList() << respone.vegetablesCellidList();
             out << respone.tariffType() << respone.balance();
             out << respone.blackNameList() << respone.orderBigCells() << respone.orderMiddleCells()\
                 << respone.orderSmallCells() << respone.orderVegeCells() << respone.orderCells();

             SCDatBase::instance()->setBoxCount(respone.bigCellIdList().size(),respone.middleCellIdList().size(),respone.smallCellIdList().size(),respone.vegetablesCellidList().size());

             SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_MENU,outData);  //存件
         }

     }
     else if(respone.success() == RET_TRUE && respone.cardType() == "1")
     {
         QByteArray data;
         QDataStream outInfo(&data,QIODevice::WriteOnly);
         outInfo << respone.m1CardId() << respone.getCompanyIdArrayNew() << respone.getCompanyNameArrayNew();
         SCMainCtrl::instance()->enterStatus(ST_INFO_INPUT,data);  //信息录入
     }
     else if(respone.success() == RET_TRUE && respone.cardType() == "3")
     {
          SCMainCtrl::instance()->enterStatus(ST_MANAGE,"");         //维修员界面
     }
     else if(respone.success() == RET_TRUE && respone.cardType() == "5")
     {
          SCMainCtrl::instance()->enterStatus(ST_FINANCE,respone.m1CardId().toAscii());    //财务界面
     }
     else if(respone.success() == RET_FALSE && respone.errCode() == DEVICE_ERROR)
     {
         SCMainCtrl::instance()->enterStatus(ST_DEVICE_REST,"");
     }
     else if(respone.success() == "false" && respone.errCode() == IC_FORBID_ERROR)
     {
         myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("您的会员卡已被冻结，详情请咨询客服。"));
     }
     else if(respone.success() == "false")
     {
         myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("登陆码不正确。"));
     }
     else
     {
         myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"));
     }

}

/**********************************************************************
* 函数名称： logSavePage_slot
* 功能描述： 无卡登陆到存件界面
* 输入参数：
* 输出参数：
* 修改日期        版本号     修改人	      修改内容
* -----------------------------------------------
* 2013/08/07	 V1.0
***********************************************************************/
void noiclogpage::logSavePage_slot()
{
    emit resetTime();
    //取件码发送给后台

    if(!isWorking)
    {
        isWorking = true;
        QRegExp rxpasswd("[0-9]\\d{5,7}");

        if (rxpasswd.exactMatch(this->take_edit->text().trimmed()))
        {
            if(logStatus == "STATUS1")
            {
                startOpenCellNoIcLog();
            }
            else if(logStatus == "STATUS2")
            {
                startDetainNoIcLog();
            }
            else if(logStatus == "STATUS3")
            {
                startCourierTakeNoIcLog();
            }
            else
            {
                startNoIcLog();
            }
        }
        else
        {
           this->take_edit->clear();
           myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("登陆码输入有误。"));
        }
        isWorking = false;
    }





}

void noiclogpage::cancelLogSavePage_slot()
{

    stack->setCurrentIndex(0);

    take_edit->clear();
    phone_edit->clear();

    phone_edit->setFocus();
    take_edit->setFocus();

    SCMainCtrl::instance()->enterStatus(ST_HOME, "");

}




/**********************************************************************
* 函数名称： waitconfirmByteArray
* 功能描述： 等待确认的包裹信息
* 输入参数：
* 输出参数：
* 修改日期        版本号     修改人	      修改内容
* -----------------------------------------------
* 2013/08/07	 V1.0
***********************************************************************/
QByteArray noiclogpage::waitconfirmByteArray(m1CardLoginResp &respone,QString personType,bool isFix)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);

    for(int i = 0; i < respone.getArrayNew().size();i++ )
    {
        out << respone.m1CardId();
        out << respone.getArrayNew().at(i)->packageId();
        out << respone.getArrayNew().at(i)->mobilePhone();
        out << respone.getArrayNew().at(i)->scellId();
        out << respone.getArrayNew().at(i)->deliveryId();  //派件 与packageid绑定的，通过发送这个给后台
        out << isFix;
        out << respone.bigCellIdList();
        out << respone.middleCellIdList();
        out << respone.smallCellIdList();
        out << respone.vegetablesCellidList();
        out << respone.tariffType();
        out << respone.balance();
        out << personType;
        out << respone.blackNameList();
        out << respone.orderBigCells();
        out << respone.orderMiddleCells();
        out << respone.orderSmallCells();
        out << respone.orderVegeCells();
        out << respone.orderCells();

    }
    return data;
}

void noiclogpage::initPage(QByteArray data)
{
    isWorking = false;

    logStatus = QString(data);
    take_edit->clear();
    phone_edit->clear();

    phone_edit->setFocus();
    take_edit->setFocus();

    stack->setCurrentIndex(0);
}



/**********************************************************************
* 函数名称： savecellByteArray
* 功能描述： 获取箱子信息
* 输入参数：
* 输出参数：
* 修改日期        版本号     修改人	      修改内容
* -----------------------------------------------
* 2013/08/07	 V1.0
***********************************************************************/
QByteArray noiclogpage::savecellByteArray(QByteArray param)  //param M1卡卡号
{
    getCellIdsByTerminalIdResp respone;
    SCServerComm::instance()->getCellIdsByTerminalId(&respone);

    while(respone.success() == "false")
    {
        SCServerComm::instance()->getCellIdsByTerminalId(&respone);
    }

    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out << respone.bigCellIdList();
    out << respone.middleCellIdList();
    out << respone.smallCellIdList();
    out << QString(param);

    return data;

}


