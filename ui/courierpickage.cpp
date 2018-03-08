#include "courierpickage.h"
#include "parser.h"
#include "servercomm.h"
#include "db.h"
#include "driver.h"
#include "exceptioncomm.h"
#include "mainctrl.h"
#include <QLabel>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>

#define         PACKAGEID      0             //快递单号
#define         MOBILEPHONE    1             //手机号码
#define         SAVETIME       2             //存入时间
#define         CMARK          3             //柜号
#define         BMARK          4             //箱号
#define         PSTATUS        5             //派件状态
#define         DELIVERYID     6             //派件ID
#define         CELLTYPE       7             //格子类型


using namespace QJson;

courierpickpage::courierpickpage(QWidget *parent) :
    QDialog(parent)
{
    initPage();
}



void courierpickpage::initPage()
{
    this->setObjectName("QWidget");
    this->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800,600);


    QLabel *label = new QLabel(this);
    label->resize(174,44);
    label->move(330,10);
    label->setStyleSheet("border-image: url(:/page/images/cardtake/listId.png)");

    QStringList list;
    list << "快递单号" << "手机号码" << "存入时间" << "柜号" << "箱号" << "派件（箱门）状态";
    view = new QTableView(this);
    view->horizontalHeader()->setFixedHeight(50);
    view->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->resize(800,350);
    view->move(0,50);
    view->setFocusPolicy(Qt::NoFocus);


    QLabel *Label = new QLabel(tr("总计/个："),this);
    Label->resize(200,40);
    Label->move(500,400);
    Label->setAlignment(Qt::AlignCenter);
    Label->setStyleSheet("font-family:wenquanyi;font-size:24px;color:rgb(255,255,255)");

    numberLabel = new QLabel(this);
    numberLabel->resize(100,40);
    numberLabel->move(700,400);
    numberLabel->setAlignment(Qt::AlignLeft);
    numberLabel->setStyleSheet("font-family:wenquanyi;font-size:24px;color:rgb(255,0,0)");

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

    connect(openBtn,SIGNAL(clicked()),this,SLOT(okBtn_slots()));
    connect(quitBtn,SIGNAL(clicked()),this,SLOT(quit_slots()));
    connect(&myMessageBox,SIGNAL(message_ok()),this,SLOT(openCell_slots()));

}

void courierpickpage::readInfo(QByteArray data)
{
    Parser  parser;
    QStringList singleData;
    deliveryTimeoutResp ret;
    QObjectHelper::qvariant2qobject(parser.parse(data).toMap(), &ret);

    modelData.clear();
    imageList.clear();
    terminal = ret.getDeliveryInfoList().at(0)->scabId();
    terminal.chop(2);

    openBtn->setEnabled(true);
    openBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/cardtake/takeBtn.png)");

    for(int i = 0; i < ret.getDeliveryInfoList().size(); i++)
    {
        singleData.clear();

        if(againCheckCellProc(ret.getDeliveryInfoList().at(i)->scellId(),ret.getDeliveryInfoList().at(i)->deliveryId()))
        {
            singleData.append(ret.getDeliveryInfoList().at(i)->packageId());         //快递单号
            singleData.append(ret.getDeliveryInfoList().at(i)->mobilePhone());       //手机号码
            singleData.append(ret.getDeliveryInfoList().at(i)->createTime());        //存入时间
            singleData.append(ret.getDeliveryInfoList().at(i)->scellId().right(4).left(2)); //柜号
            singleData.append(ret.getDeliveryInfoList().at(i)->scellId().right(2));  //箱号
            singleData.append(QString("存入"));
            singleData.append(ret.getDeliveryInfoList().at(i)->deliveryId());        //派件编号
            singleData.append(ret.getDeliveryInfoList().at(i)->scellType());         //格子类型大中小蔬菜

            modelData.append(singleData);
        }

    }

    numberLabel->setText(QString("%1").arg(modelData.size()));
    model->setModelData(modelData);
    model->refrushModel();
    view->setModel(model);

}


bool courierpickpage::againCheckCellProc(QString openCell,QString deliveryId)
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
                    return false;
                }
            }
        }

        if(serverlist.at(i).errType == "2")   //件已被用户取走
        {
            if(serverlist.at(i).cellId == deliveryId)
            {
                return false;
            }
        }

        if(serverlist.at(i).errType == "5")
        {
            list = serverlist.at(i).cellId.split("|"); //件被快递员取走 XX|XX
            for(int j = 0; j < list.size(); j++)
            {
                if(list.at(j) == deliveryId)
                {
                    return false;
                }
            }
        }
    }

    return true;
}


void courierpickpage::okBtn_slots()
{
    emit resetTime();
    myMessageBox.myPrompt(QMessageBox::Warning,tr("提示确认"),tr("您将取出所有滞留件，请注意取出快件。"));
}


void courierpickpage::openCell_slots()
{
    emit resetTime();
    openBtn->setEnabled(false);
    quitBtn->setEnabled(false);
    openBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/cardtake/takeBtn_bg.png)");

    int length = modelData.size();
    qint8 cabinetId = 0;
    qint8 cellId = 0;
    qint8 cellType = 0;

    bool video = true;

    QString tempErrType = "2";

    QStringList list;
    QString cellStatus;

    while(length)
    {
        cabinetId = modelData.at(length-1).at(CMARK).toInt();     //柜号
        cellId    = modelData.at(length-1).at(BMARK).toInt();     //箱号
        cellType  = modelData.at(length-1).at(CELLTYPE).toInt();  //箱子类型
        cellStatus = modelData.at(length-1).at(PSTATUS); //箱门状态

        //刷新模型数据
        list = modelData.at(length-1);

        if(cellStatus == QString(tr("存入")))
        {
            if(lockoperation.openLock(cabinetId,cellId) == 1)
            {
                if(video)
                {
                    video = false;
                    //打开摄像头
                    SCDriver::instance()->catchVideo(modelData.at(length-1).at(DELIVERYID));
                }

                //插入打开的箱门到数据库
                SCDatBase::instance()->insertOpenException(cabinetId,cellId,cellType,tempErrType);

                list.replace(PSTATUS,QString("取出"));
                modelData.replace(length-1,list);
            }
            else
            {
                 list.replace(PSTATUS,QString("等待维修"));
                 modelData.replace(length-1,list);
            }
        }
        length = length - 1;

    }

    model->setModelData(modelData);
    model->refrushModel();
    view->setModel(model);

    reportInfo();

    openBtn->setEnabled(false);
    quitBtn->setEnabled(true);

}



void courierpickpage::reportInfo()
{
    QList<cellErrorInfo> boxExceptionList;  //打不开箱门列表

    int length = modelData.size();

    QStringList list;
    QString cellStatus;
    QStringList takePage;

    while(length)
    {
        cellStatus = modelData.at(length-1).at(PSTATUS); //箱门状态

        //刷新模型数据
        list = modelData.at(length-1);

        if(cellStatus == QString("取出"))
        {
            takePage.append(list.at(DELIVERYID));
        }
        if(cellStatus == QString(tr("等待维修")))
        {
            cellErrorInfo notOpenInfo;

            notOpenInfo.scabId = terminal + list.at(CMARK);
            notOpenInfo.scellId = terminal + list.at(CMARK) + list.at(BMARK);
            notOpenInfo.errType = "4";
            notOpenInfo.relDeliveryId = list.at(DELIVERYID);;
            boxExceptionList.append(notOpenInfo);
        }
            length = length - 1;
    }

    imageList = takePage;

    for(int i = 0; i < takePage.size(); i++)
    {
        //删除本地存储信息
        SCDatBase::instance()->deleteTakePackageInfo(takePage.at(i));
    }

    if(takePage.size() > 0)
    {
        expTakeDeliveryResp respone;
        SCServerComm::instance()->expTakeDelivery(takePage,&respone);
    }

    if(boxExceptionList.size() > 0)
    {
        reportCellErrorsResp ret;
        SCExceptioncomm::instance()->reportCellErrors(boxExceptionList,&ret);
    }

    courierTakeInfoPage.setInfo(imageList.size(),boxExceptionList.size(),0,0,false);

}




void courierpickpage::quit_slots()
{
    QString Image;

    if(imageList.size() > 0)
    {
        for(int i = 0; i < imageList.size(); i++)
        {
            Image = Image + imageList.at(i) + "-";
        }

        Image.chop(1);
        Image = "/usr/lxyz/picture/" + Image + ".gif";

        QString oldImage = "/usr/lxyz/picture/" + imageList.at(0) + ".gif";
        QString temp = QString("mv %1 %2").arg(oldImage).arg(Image);
        QByteArray imageData = temp.toLatin1();
        system(imageData.data());
    }

    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
