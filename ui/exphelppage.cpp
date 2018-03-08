#include "exphelppage.h"
#include "ui_exphelppage.h"
#include "parser.h"
#include "mainctrl.h"
#include "db.h"
#include "driver.h"
#include "servercomm.h"
#include "exceptioncomm.h"
#include <QLabel>


#define         PACKAGEID      0             //快递单号
#define         MOBILEPHONE    1             //手机号码
#define         SAVETIME       2             //存入时间
#define         CMARK          3             //柜号
#define         BMARK          4             //箱号
#define         PSTATUS        5             //打开按钮
#define         DELIVERYID     6             //派件ID
#define         CELLTYPE       7             //格子类型



using namespace QJson;

exphelppage::exphelppage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::exphelppage)
{
    ui->setupUi(this);

    initPage();
}

exphelppage::~exphelppage()
{
    delete ui;
}


void exphelppage::initPage()
{

    this->setObjectName("QWidget");
    this->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800,600);


    QLabel *label = new QLabel(this);
    label->resize(240,44);
    label->move(310,10);
    label->setStyleSheet("border-image: url(:/page/images/exception/packId.png)");

    QStringList list;
    list << "快递单号" << "收件人手机号码" << "存入时间" << "柜号" << "箱号" << "取件";
    view = new QTableView(this);
    view->horizontalHeader()->setFixedHeight(50);
    view->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    view->resize(800,350);
    view->move(0,50);
    view->setFocusPolicy(Qt::NoFocus);

    QLabel *Label = new QLabel(tr("总计/个："),this);
    Label->resize(200,40);
    Label->move(500,400);
    Label->setAlignment(Qt::AlignCenter);
    Label->setStyleSheet("font-family:wenquanyi;font-size:24px;color:rgb(255,255,255)");

    packCount = new QLabel(this);
    packCount->resize(100,40);
    packCount->move(700,400);
    packCount->setAlignment(Qt::AlignLeft);
    packCount->setStyleSheet("font-family:wenquanyi;font-size:24px;color:rgb(255,0,0)");


    exitBtn = new QPushButton(this);
    exitBtn->resize(182,70);
    exitBtn->move(320,500);
    exitBtn->setFlat(true);
    exitBtn->setFocusPolicy(Qt::NoFocus);
    exitBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/exitBtn.png)");

    model = new takeViewModel();
    model->setHorizontalHeaderList(list);

    btnDelegate = new BtnDelegate(this);
    view->setItemDelegate(btnDelegate);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);


    connect(exitBtn,SIGNAL(clicked()),this,SLOT(exit_slots()));
    connect(btnDelegate,SIGNAL(btnClick(int,int)),SLOT(openCell_slots(int,int)));
}


void exphelppage::readInfo(QByteArray data)
{
    Parser  parser;
    QStringList singleData;
    expGetSaveDeliveryResp ret;
    QObjectHelper::qvariant2qobject(parser.parse(data).toMap(), &ret);

    modelData.clear();
    takeDeliveryList.clear();
    notOpenCell.clear();
    terminal = ret.getDeliveryInfoList().at(0)->scabId();
    terminal.chop(2);

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
            singleData.append(QString("点击取出"));
            singleData.append(ret.getDeliveryInfoList().at(i)->deliveryId());        //派件编号
            singleData.append(ret.getDeliveryInfoList().at(i)->scellType());         //格子类型大中小蔬菜

            modelData.append(singleData);
        }

    }

    packCount->setText(QString("%1").arg(modelData.size()));
    model->setModelData(modelData);
    model->refrushModel();
    view->setModel(model);


    view->setItemDelegate(btnDelegate);
    view->setEditTriggers(QAbstractItemView::AllEditTriggers);  

}



bool exphelppage::againCheckCellProc(QString openCell,QString deliveryId)
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


void exphelppage::openCell_slots(int row, int coloum)
{
    exitBtn->setEnabled(false);
    emit resetTime();
    QString tempErrType = "2";
    QStringList list = modelData.at(row);
    if(lockoperation.openLock(list.at(CMARK).toInt(),list.at(BMARK).toInt()) == 1)
    {
        takeDeliveryList.append(list.at(DELIVERYID));

        SCDatBase::instance()->deleteTakePackageInfo(list.at(DELIVERYID));

        SCDriver::instance()->catchVideo(list.at(DELIVERYID));

        //插入打开的箱门到数据库
        SCDatBase::instance()->insertOpenException(list.at(CMARK).toInt(),list.at(BMARK).toInt(),list.at(CELLTYPE).toInt(),tempErrType);


        list.replace(PSTATUS,QString("已取出"));
        modelData.replace(row,list);
    }
    else
    {
         QString cell = terminal + list.at(CMARK) + list.at(BMARK);

         notOpenCell.insert(cell,list.at(DELIVERYID));
         list.replace(PSTATUS,QString("打不开"));
         modelData.replace(row,list);
    }

    model->setModelData(modelData);
    model->refrushModel();
    view->setModel(model);
    exitBtn->setEnabled(true);
}


void exphelppage::exit_slots()
{
    exitBtn->setEnabled(false);
    if(takeDeliveryList.size() > 0)
    {
        expTakeDeliveryResp respone;
        SCServerComm::instance()->expTakeDelivery(takeDeliveryList,&respone);
    }

    QList<cellErrorInfo> boxExceptionList;  //打不开箱门列表

    QMap<QString, QString>::iterator it;
    for (it = notOpenCell.begin(); it != notOpenCell.end(); ++it)
    {
        QString cell = it.key();
        cellErrorInfo notOpenInfo;

        notOpenInfo.scabId = cell.left(cell.size() - 1);
        notOpenInfo.scellId = cell;
        notOpenInfo.errType = "4";
        notOpenInfo.relDeliveryId = it.value();
        boxExceptionList.append(notOpenInfo);
    }

    if(boxExceptionList.size() > 0)
    {
        reportCellErrorsResp ret;
        SCExceptioncomm::instance()->reportCellErrors(boxExceptionList,&ret);
    }
    exitBtn->setEnabled(true);
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
