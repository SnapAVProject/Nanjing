#include "homestatus.h"
#include "mainctrl.h"
#include "respone.h"
#include "driver.h"
#include "servercomm.h"
#include "m1CardLoginResp.h"
#include "getCellIdsByTerminalIdResp.h"
#include "configure/db.h"
#include <QFile>
#include <QDataStream>
#include <QDateTime>


CHomeStatus::CHomeStatus(STATUS status):
     CStatus(status)
{
      mPage = new HomePage();
      myMessageBox = new CMessageBox(3*1000);

      connect(this,SIGNAL(cardcoming(QString)),mPage,SLOT(close_promptPage()));
      connect(&myBox,SIGNAL(message_ok()),this,SLOT(takeDelayPackage()));

}

void CHomeStatus::enter(QByteArray param)
{
    //qDebug() << " enter home statuse " <<  param;

    if(param == "REFRESHCELL") //刷新可用格子
    {
        mPage->upDataCellInfo();
    }
    mPage->show();
}


void CHomeStatus::leave(QByteArray param)
{
    //qDebug() << " leave homestatus" << param;
    //mPage->hide();
}

void CHomeStatus::updateCell()
{
    mPage->upDataCellInfo();
}


void CHomeStatus::coinInputProc(QByteArray param)
{
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

/**********************************************************************
* 函数名称： takeDelayPackage
* 功能描述： 快递员取滞留件
* 输入参数：
* 输出参数：
* 修改日期        版本号     修改人	      修改内容
* -----------------------------------------------
* 2014/08/011	 V1.0
***********************************************************************/

void CHomeStatus::takeDelayPackage()
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
         myMessageBox->myPrompt(QMessageBox::Information,tr("提示"),tr("您没有滞留件需要处理。"));
    }
    else if(respone.errCode() == "10009")
    {
         myMessageBox->myPrompt(QMessageBox::Information,tr("提示"),tr("卡号不存在。"));
    }

}


void CHomeStatus::cardInputProc(QByteArray param)
{
    //进入其他状态
    //SCMainCtrl::instance()->enterStatus();
    //qDebug() << "now card is coming" << param;
    emit cardcoming(QString(param));

    mPage->setBtnEnable(false);

    if(specialcard(param))
    {

        SCMainCtrl::instance()->enterStatus(ST_SET_PORT,"");
    }
    else
    {
        m1CardLoginResp  respone;

        progress.setMinimum(0);
        progress.setMaximum(0);
        progress.setValue(0);
        progress.setCancelButton(0);
        progress.setWindowFlags(Qt::FramelessWindowHint);
        progress.setLabelText(tr("处理中，请稍后..."));
        progress.show();

        SCServerComm::instance()->m1CardLogin(QString(param),&respone);

        progress.hide();

        mM1Card = QString(param);

        if(respone.success() == RET_FALSE && respone.errCode() == IC_FORBID_ERROR )
        {
            myMessageBox->myPrompt(QMessageBox::Warning,tr("提示"),tr("您的会员卡已被冻结，详情请咨询客服。"));
        }
        else if(respone.success() == RET_FALSE && respone.errCode() == TRANS_KEY_ERROR)
        {
            myMessageBox->myPrompt(QMessageBox::Warning,tr("提示"),tr("通信错误，请联系客服。"));
        }
        else if(respone.success() == RET_FALSE && respone.errCode() == DEVICE_ERROR)
        {
            SCMainCtrl::instance()->enterStatus(ST_DEVICE_REST,"");
        }
        else if(respone.success() == "false")
        {

            myMessageBox->myPrompt(QMessageBox::Warning,tr("提示"),tr("卡号未注册。"));
            // SCServerComm::instance()->m1CardLogin(QString(param),timeral,&respone);
        }
        else if(respone.success() == RET_TRUE && respone.errCode() == OVERDUE_PACKAGE )
        {
            mMobilePhone = respone.mobilePhone();
            myBox.myPrompt(QMessageBox::Warning,tr("提示确认"),tr("您存在滞留快件，取出后才可继续存件，是否取出？"));
        }
        else if(respone.success() == "true")
        {
            int dec = respone.cardType().toInt();
            double balance = respone.balance().toDouble();
            bool isFix = false;
            if( !respone.orderBigCells().isEmpty() || !respone.orderMiddleCells().isEmpty() || !respone.orderSmallCells().isEmpty() || !respone.orderVegeCells().isEmpty())
            {
                isFix = true;
            }
            // qDebug() << " card  dec: :" << dec;

            switch(dec)
            {
            case 1:
            {
                QByteArray data;
                QDataStream outInfo(&data,QIODevice::WriteOnly);
                outInfo << QString(param) << respone.getCompanyIdArrayNew() << respone.getCompanyNameArrayNew();
                SCMainCtrl::instance()->enterStatus(ST_INFO_INPUT,data);  //信息录入
                break;
            }
            case 4:
                //蔬菜
                // break;
            case 2:
            {
                QString personType;
                if(2 == dec)
                {
                    personType = "KD";
                }
                else
                {
                    personType = "SC";
                }

                if(respone.getArrayNew().size() != 0 /*respone.getArrayNew().at(0)->pakeageId().size() != 0*/)
                {
                    SCMainCtrl::instance()->enterStatus(ST_PAKEAGE_CONFIRM,waitconfirmByteArray(respone,personType,isFix));  //存件确认界面
                }
                //检查余额是否充足，不充足给出提示，充足进入存件界面

                else if(((respone.tariffType().toInt() == 2)||(respone.tariffType().toInt() == 4))&& !isFix && balance <= 0)
                {
                    myMessageBox->myPrompt(QMessageBox::Warning,tr("提示"),tr("该系统存件需付费，请您先充值。"));
                }
                else if(((respone.tariffType().toInt() == 2)||(respone.tariffType().toInt() == 4))&& isFix && balance < 0)
                {
                    myMessageBox->myPrompt(QMessageBox::Warning,tr("提示"),tr("该系统存件需付费，请您先充值。"));
                }
                else
                {

                    QByteArray     outData;
                    QDataStream out(&outData,QIODevice::WriteOnly);

                    out << QString(param) << personType << respone.bigCellIdList() << respone.middleCellIdList() \
                        << respone.smallCellIdList() << respone.vegetablesCellidList() << respone.tariffType() \
                        << respone.balance() << respone.blackNameList() << respone.orderBigCells() << respone.orderMiddleCells()\
                        << respone.orderSmallCells() << respone.orderVegeCells() << respone.orderCells();

                    SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_MENU,outData);  //存件
                }
            }
                break;
            case 3:
                SCMainCtrl::instance()->enterStatus(ST_MANAGE,"");         //维修员界面
                break;
            case 5:
                SCMainCtrl::instance()->enterStatus(ST_FINANCE,param);
                break;
            default:
                myMessageBox->myPrompt(QMessageBox::Warning,tr("提示"),tr("VIP用户刷卡取件请点击取按钮再刷卡。"));
                break;
            }
        }
        else
        {
            myMessageBox->myPrompt(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"));
        }
    }
    mPage->setBtnEnable(true);
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
QByteArray CHomeStatus::waitconfirmByteArray(m1CardLoginResp &respone,QString personType,bool isFix)
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);

   // qDebug() << "wait connnnnfirm coming respone:" << respone.m1CardId();
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


/**********************************************************************
* 函数名称： savecellByteArray
* 功能描述： 获取箱子信息
* 输入参数：
* 输出参数：
* 修改日期        版本号     修改人	      修改内容
* -----------------------------------------------
* 2013/08/07	 V1.0
***********************************************************************/
QByteArray CHomeStatus::savecellByteArray(QByteArray param)
{
    getCellIdsByTerminalIdResp respone;
    SCServerComm::instance()->getCellIdsByTerminalId(&respone);

    if(respone.success() == "false")
    {
      //  qDebug() << "getCellIdsByTerminalId is error";
      //  SCServerComm::instance()->getCellIdsByTerminalId(terminal,&respone);
    }

    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out << respone.bigCellIdList();
    out << respone.middleCellIdList();
    out << respone.smallCellIdList();
    out << QString(param);

    return data;

}


/**********************************************************************
* 函数名称： specialcard
* 功能描述： 判断卡号是否是特别卡（可以设置端口号）
* 输入参数：
* 输出参数：
* 修改日期        版本号     修改人	      修改内容
* -----------------------------------------------
* 2013/08/07	 V1.0
***********************************************************************/

bool CHomeStatus::specialcard(QByteArray param)
{
    QStringList list;
    bool ok = false;
    QFile f("IPSetCard.txt");
    if(f.open(QIODevice::ReadOnly))
    {
        QTextStream in(&f);
        while(!in.atEnd())
        {
            list.append(in.readLine());
        }
        for(int i = 0;i < list.size();i++)
        {
            //qDebug() << "special card:" << list.at(i);
            if(list.at(i) == QString(param).toUpper())
            {
                ok = true;
                break;
            }

        }

    }
    else
    {
        //qDebug() << "open error";
        ok = false;
    }

    f.close();
    return ok;
}
