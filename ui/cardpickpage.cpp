#include "cardpickpage.h"
#include "parser.h"
#include "mainctrl.h"
#include "db.h"
#include "driver.h"
#include "servercomm.h"
#include "exceptioncomm.h"
#include <QPushButton>
#include <QTableView>
#include <QHeaderView>
#include <QLabel>
#include <QDebug>


//***************************************************************************
/**file		cardpickpage.cpp
* brief		刷卡取件
* author	hjzhang@163.com	2014/4/8
* version	1.0
*/
//***************************************************************************

#define    ROW          7

#define    PACKAGE      0     //快递单号
#define    BCASH        1     //基础费用
#define    DCASH        2     //滞留费用
#define    PCASH        3     //已付费用
#define    CMARK        4     //柜号
#define    BMARK        5     //箱号
#define    PSTATUS      6     //派件（箱门）状态
#define    DELIVERID    7     //派件ID
#define    CELLTYPE     8     //箱子类型
#define    WILLPCASH    9     //单见将付款金额(自费）

using namespace QJson;

cardpickpage::cardpickpage(QWidget *parent) :
    QDialog(parent)
{
    initPage();
}


void cardpickpage::initPage()
{
    this->setObjectName("QWidget");
    this->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800,600);


    QLabel *label = new QLabel(this);
    label->resize(174,44);
    label->move(330,10);
    label->setStyleSheet("border-image: url(:/page/images/cardtake/packageId.png)");

    QStringList list;
    list << "快递单号" << "基础费用/元" << "滞留费用/元" << "已付费用/元" << "柜号" << "箱号" << "派件（箱门）状态";
    view = new QTableView(this);
    view->horizontalHeader()->setFixedHeight(50);
    view->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->resize(800,350);
    view->move(0,50);
    view->setFocusPolicy(Qt::NoFocus);

    QLabel *payLabel = new QLabel(tr("付费预估/元（此界面暂不支持投币）："),this);
    payLabel->resize(400,40);
    payLabel->move(300,400);
    payLabel->setAlignment(Qt::AlignCenter);
    payLabel->setStyleSheet("font-family:wenquanyi;font-size:24px;color:rgb(255,255,255)");

    payInfo = new QLabel(this);
    payInfo->resize(100,40);
    payInfo->move(700,400);
    payInfo->setAlignment(Qt::AlignCenter);
    payInfo->setStyleSheet("font-family:wenquanyi;font-size:24px;color:rgb(255,0,0)");

    openBtn = new QPushButton(this);
    openBtn->resize(182,70);
    openBtn->move(170,494);
    openBtn->setFlat(true);
    openBtn->setFocusPolicy(Qt::NoFocus);
    openBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/cardtake/takeBtn.png)");

    quitBtn = new QPushButton(this);
    quitBtn->resize(182,70);
    quitBtn->move(453,494);
    quitBtn->setFlat(true);
    quitBtn->setFocusPolicy(Qt::NoFocus);
    quitBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/exitBtn.png)");

    model = new takeViewModel();
    model->setHorizontalHeaderList(list);

    connect(openBtn,SIGNAL(clicked()),this,SLOT(openBox()));
    connect(quitBtn,SIGNAL(clicked()),this,SLOT(quitPage()));
    connect(&takeInfoPage,SIGNAL(closeInfoPage()),this,SLOT(quitPage()));

}


void cardpickpage::readInfo(QByteArray data)
{
    openBtn->setEnabled(true);
    quitBtn->setEnabled(true);
    openBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/cardtake/takeBtn.png)");

    Parser  parser;
    QStringList singleData;
    getDelsByVIPCardResp ret;
    QObjectHelper::qvariant2qobject(parser.parse(data).toMap(), &ret);

    QString deliveryId;
    QString openCell;
    int boxStatus = 0;
    double baseCost = 0;
    double detainCost = 0;
    double paidCost = 0;
    double willPaidCost = 0; //将付多少钱取走件(自费）

    int count = 0;
    int allCount = 0;
    payCoinCard = 0;

    debtCost = 0;    //自费的总费用
    vipDebtCost = 0; //年卡的总费用
    modelData.clear();
    openCellList.clear();
    closeCellList.clear();

    terminal = SCDatBase::instance()->getTerminalId();
    m1Card = ret.m1Card();
    mCardDate = ret.isExpired();
    mBalance = ret.balance().toDouble();
    vipRate = ret.vipPercent().toDouble();


    for(int i = 0; i < ret.getDeliveryInfo().size(); i++)
    {
        singleData.clear();

        deliveryId = ret.getDeliveryInfo().at(i)->deliveryId();
        openCell = ret.getDeliveryInfo().at(i)->scellId();
        boxStatus = ret.getDeliveryInfo().at(i)->scellStatus().toInt();
        baseCost = ret.getDeliveryInfo().at(i)->delOwnerCost().toDouble();
        detainCost = ret.getDeliveryInfo().at(i)->debtCost().toDouble();
        paidCost = ret.getDeliveryInfo().at(i)->ownerHasPay().toDouble();

        //显示派件列表信息
        if(againCheckCellProc(boxStatus,paidCost,deliveryId,openCell) && (boxStatus != 3))
        {
            willPaidCost = QString::number(baseCost + detainCost - paidCost,'f',2).toDouble();
            debtCost = debtCost + QString::number(baseCost + detainCost - paidCost,'f',2).toDouble();
            vipDebtCost = vipDebtCost + QString::number(detainCost - paidCost,'f',2).toDouble();      

            singleData.append(ret.getDeliveryInfo().at(i)->packageId());  //单号
            singleData.append(QString("%1").arg(baseCost));  //基础费
            singleData.append(QString("%1").arg(detainCost));    //滞留费
            singleData.append(QString("%1").arg(paidCost));  //已付
            singleData.append(openCell.right(4).left(2)); //柜号
            singleData.append(openCell.right(2));  //箱号
            if(0 == boxStatus)                //派件状态
                singleData.append(QString("存入"));
            else if((1 == boxStatus) || (4 == boxStatus))
            {
                count++;
                singleData.append(QString("等待维修"));
            }
            else if(2 == boxStatus)
                singleData.append(QString("未关闭"));
            else
                singleData.append(QString("取出"));

            singleData.append(deliveryId);  //派件ID
            singleData.append(ret.getDeliveryInfo().at(i)->scellType());  //箱子类型 1、大 2、中 3、小 4、蔬菜
            singleData.append(QString("%1").arg(willPaidCost));      ////将付多少钱取走件(自费）

            allCount++;
            modelData.append(singleData);
        }

    }

    if(mCardDate == "no")
    {
        if(vipDebtCost < 0)
        {
            vipDebtCost = 0;
        }
        payInfo->setText(QString("%1").arg(vipDebtCost*vipRate));
    }
    else
    {
        payInfo->setText(QString("%1").arg(debtCost*vipRate));
    }

    if(count == allCount)
    {
        openBtn->setEnabled(false);
        openBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/cardtake/takeBtn_bg.png)");
    }

        model->setModelData(modelData);
        model->refrushModel();
        view->setModel(model);
}


/***************************************************************

        函数名称：againCheckCellProc()
        函数功能：过滤派件信息
        函数参数： boxStatus：箱子（派件）状态 0.正常 1、打不开 2.未关闭 3、取走  paidCost：已付费信息
                   deliveryId：派件ID     openCell：箱门号 （00000001000000020102）
        返回值：无

***************************************************************/

bool cardpickpage::againCheckCellProc(int &boxStatus, double &paidCost,QString deliveryId,QString openCell)
{
    QStringList list;
    QList<ServerDesc_t> serverlist;
    QList<DebtDesc_t> debtlist;
    bool ret = true;


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
                    boxStatus = 1;
                    break;
                }
            }
        }

        else if(serverlist.at(i).errType == "2")   //件已被取走
        {
            if(serverlist.at(i).cellId == deliveryId)
            {
                boxStatus = 3;
                ret = false;
            }
        }
        else if(serverlist.at(i).errType == "5")   //件已快递员被取走
        {
            if(serverlist.at(i).cellId == deliveryId)
            {
                boxStatus = 3;
                ret = false;
            }
        }
    }

    for(int k = 0;k < debtlist.size();k++)   //欠费钱
    {
        if(debtlist.at(k).deliveryId == deliveryId)
        {
            paidCost = paidCost + debtlist.at(k).debt;
            break;
        }
    }

    return ret;

}


/***************************************************************

        函数名称：updateModelData()
        函数功能：跟改派件信息
        函数参数： row：行  column：列  value：改变后的值
        返回值：无

***************************************************************/
void cardpickpage::updateModelData(int row,int column,QVariant value)
{
    model->setData(model->index(row,column),value,Qt::EditRole);
}

/***************************************************************

        函数名称：openLock()
        函数功能：执行开锁
        函数参数： 无
        返回值：无

***************************************************************/
int cardpickpage::startOpenLock(bool way)
{
    int length = modelData.size();
    qint8 cabinetId = 0;
    qint8 cellId = 0;
    qint8 cellType = 0;
    bool video = true;

    double baseCoin = 0;  //基础费
    double detainCoin = 0; //滞留费
    double payCoin = 0;   //已付
    double willPayCoin = 0;

    QStringList list;
 //   QString image;
    QString cellStatus;
    image.clear();

    QString tempErrType = "2";

    if(!length)
        return -1;

    advertisementPage.show();

    while(length)
    {
        cabinetId = modelData.at(length-1).at(CMARK).toInt();     //柜号
        cellId    = modelData.at(length-1).at(BMARK).toInt();     //箱号
        cellType  = modelData.at(length-1).at(CELLTYPE).toInt();  //箱子类型
        cellStatus = modelData.at(length-1).at(PSTATUS); //箱门状态
        //刷新模型数据(已付费用、派件状态、及付款金额）
        list = modelData.at(length-1);

        if(cellStatus == QString(tr("存入")))
        {
            if(lockoperation.openLock(cabinetId,cellId) == 1)
            {
                if(video)
                {
                    video = false;
                    //打开摄像头
                    image = "/usr/lxyz/picture/" + modelData.at(length-1).at(DELIVERID) + ".gif";
                    SCDriver::instance()->catchVideo(modelData.at(length-1).at(DELIVERID));
                }

                baseCoin = modelData.at(length-1).at(BCASH).toDouble();
                detainCoin = modelData.at(length-1).at(DCASH).toDouble();
                payCoin = modelData.at(length-1).at(PCASH).toDouble();
                willPayCoin = modelData.at(length-1).at(WILLPCASH).toDouble();

                list.replace(PSTATUS,QString("取出"));

                if(way)
                {
                    //年卡
                    if(detainCoin >= payCoin)
                    {
                        list.replace(PCASH,QString::number((detainCoin - payCoin)*vipRate + payCoin,'f',2));
                        list.replace(WILLPCASH,QString::number((detainCoin - payCoin)*vipRate,'f',2));
                    }
                    else
                    {
                         list.replace(WILLPCASH,QString::number(0*vipRate,'f',2));
                    }
                }
                else
                {
                    //自费
                    list.replace(PCASH,QString::number(willPayCoin*vipRate + payCoin,'f',2));
                    list.replace(WILLPCASH,QString::number(willPayCoin*vipRate,'f',2));
                }

                //qDebug() << "payCoinCard:" << payCoinCard << "list.at(WILLPCASH).toDouble():" << list.at(WILLPCASH).toDouble();
                payCoinCard = payCoinCard + list.at(WILLPCASH).toDouble();

                modelData.replace(length-1,list);
                openCellList.insert(length-1,modelData.at(length-1));

                //插入打开的箱门到数据库
                SCDatBase::instance()->insertOpenException(cabinetId,cellId,cellType,tempErrType);
            }
            else
            {
                list.replace(PSTATUS,QString("打不开"));
                modelData.replace(length-1,list);
                closeCellList.insert(length-1,modelData.at(length-1));
            }
        }
        length = length - 1;
    }

    if(!video)
    {

        //播放音乐
        QString mp3file = "./music/take.mp3";
        QString vol = "15";
        SCDriver::instance()->playVoice(mp3file,vol);
    }

    takeInfoPage.setInfo(openCellList.size(),closeCellList.size(),payCoinCard,mBalance);

    return 0;
}

void cardpickpage::openBox()
{
    emit resetTime();
    bool ret = true;
    openBtn->setEnabled(false);
    quitBtn->setEnabled(false);

    if("no" == mCardDate)
    {
        if(mBalance >= vipDebtCost)
        {
            if(startOpenLock(ret) == -1)
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("该终端没有您的包裹。"));
            }           
        }
        else
        {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("会员卡余额不足，请单个取件或充值。"));
        }
    }
    else if("yes" == mCardDate)
    {
        ret = false;

        if(mBalance >= debtCost)
        {
            if(startOpenLock(ret) == -1)
            {
                myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("该终端没有您的包裹。"));
            }
        }
        else
        {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("会员卡余额不足，请单个取件或充值。"));
        }
    }

    model->setModelData(modelData);
    model->refrushModel();
    view->setModel(model);

    reportInfo();

    openBtn->setEnabled(true);
    quitBtn->setEnabled(true);
}


/***************************************************************

        函数名称：reportInfo()
        函数功能：上报信息
        函数参数： 无
        返回值：无

***************************************************************/
void cardpickpage::reportInfo()
{

    QString deliveryId;
    QString accountPaid;                //付多少
    QString debtCost;                   //滞流费
    QString payWay = "2";               //刷卡付(默认）
    QString cashPayMoney = "0.0";       //本次付暂时不支持投币默认0.0
    QString forMySelf = "yes";          //为自己付款
    QList<cellErrorInfo> boxExceptionList;  //打不开箱门列表
    getDeliveryConfirmResp respone;
    QStringList list;



    //成功打开的箱门
    QMap<int, QStringList>::iterator it;
    for(it = openCellList.begin(); it != openCellList.end(); it++)
    {
        list = it.value();
        deliveryId = list.at(DELIVERID);
        accountPaid = list.at(WILLPCASH);
        debtCost = list.at(DCASH);

        if(list.at(PCASH).toDouble() != accountPaid.toDouble())
        {
            payWay = "4";  //混合
        }

        //删除本地存储信息
        SCDatBase::instance()->deleteTakePackageInfo(deliveryId);
        SCServerComm::instance()->getDeliveryConfirm(deliveryId,accountPaid,debtCost,payWay,cashPayMoney,m1Card,forMySelf,mCardDate,&respone);
    }

    //打不开的箱门
    for(it = closeCellList.begin(); it != closeCellList.end(); it++)
    {
        cellErrorInfo notOpenInfo;

        list = it.value();
        notOpenInfo.scabId = terminal + list.at(CMARK);
        notOpenInfo.scellId = terminal + list.at(CMARK) + list.at(BMARK);
        notOpenInfo.errType = "3";
        notOpenInfo.relDeliveryId = list.at(DELIVERID);;
        boxExceptionList.append(notOpenInfo);
    }

    if(closeCellList.size() > 0)
    {
        reportCellErrorsResp respone;
        SCExceptioncomm::instance()->reportCellErrors(boxExceptionList,&respone);
    }
}

void cardpickpage::quitPage()
{
    if(image.size() > 0)
    {
        QStringList listTemp;
        QString allImage;
        QMap<int, QStringList>::iterator it;
        for(it = openCellList.begin(); it != openCellList.end(); it++)
        {
            listTemp = it.value();
            allImage = allImage + listTemp.at(DELIVERID) + "-";
        }
        allImage.chop(1);
        allImage = "/usr/lxyz/picture/" + allImage + ".gif";

        QString temp = QString("mv %1 %2").arg(image).arg(allImage);
        QByteArray imageData = temp.toLatin1();
        system(imageData.data());

        image.clear();
    }

    advertisementPage.close();
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
