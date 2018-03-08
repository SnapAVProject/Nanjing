#include "savepackagelistpage.h"
#include "mainctrl.h"
#include "db.h"
#include "driver.h"
#include "exceptioncomm.h"
#include <QPushButton>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>

//***************************************************************************
/**file		savepackagelistpage.cpp
* brief		存件
* author	hjzhang@163.com	2013/9/26
* version	1.3
*/
//***************************************************************************

savepackagelistpage::savepackagelistpage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    initListInfopage();
}
void savepackagelistpage::initListInfopage()
{

    QStringList list;
    list<< "序号" << "快递单号" << "用户手机号" << "柜号" << "箱号";
    showModel = new CFormTablemodel();
    showModel->setheader(list);
    packageNumber = 0;
    this->resize(800,600);
    this->setObjectName("QWidget");
    this->setStyleSheet("#QWidget{border-image: url(:/public/images/public/beijing.png)}");


    QLabel *label = new QLabel(this);
    label->resize(150,41);
    label->move(330,10);
    label->setStyleSheet("border-image: url(:/page/images/saveList/word.png)");

    savetableView = new QTableView(this);
    savetableView->resize(800,444);
    savetableView->move(0,50);
    //水平模型头充满水平区域
    QHeaderView *header;
    header = savetableView->horizontalHeader();
    header->setResizeMode(QHeaderView::Stretch);

    savetableView->verticalHeader()->hide();
    savetableView->sizeHint();

    nextBtn = new QPushButton(this);
    nextBtn->resize(182,70);
    nextBtn->move(170,494);
    nextBtn->setFlat(true);
    nextBtn->setFocusPolicy(Qt::NoFocus);
    nextBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/saveList/nextBtn.png)");

    okBtn = new QPushButton(this);
    okBtn->resize(182,70);
    okBtn->move(453,494);
    okBtn->setFlat(true);
    okBtn->setFocusPolicy(Qt::NoFocus);
    okBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/saveList/okBtn.png)");



    connect(okBtn,SIGNAL(clicked()),this,SLOT(okBtnSlots()));
    connect(nextBtn,SIGNAL(clicked()),this,SLOT(nextBtnSlots()));

}

void savepackagelistpage::setBtnEnable(bool status)
{
    nextBtn->setEnabled(status);
    okBtn->setEnabled(status);
}


void savepackagelistpage::okBtnSlots()
{
    setBtnEnable(false);
    emit timeStop();

    packageNumber = 0;

    qint8 bigBox = 0;
    qint8 middleBox = 0;
    qint8 smallBox = 0;
    qint8 vegetableBox = 0;

    qint8 cabinetId;
    qint8 cellId;
    qint8 cellType;
    bool ok;
    QByteArray byteArry;

    CellInfoDesc_t cellInfo;

    QList<cellErrorInfo> boxExceptionList;


    for(QMap<QString,qint8>::iterator it = notOpenBoxList.begin(); it != notOpenBoxList.end(); it++)
    {
        cellErrorInfo notOpenInfo;
        QString notOpenStr = it.key();
        int length = notOpenStr.size();

        if(1 == it.value())
        {
            bigBox++;
        }
        else if(2 == it.value())
        {
            middleBox++;
        }
        else if(3 == it.value())
        {
            smallBox++;
        }
        else
        {
            vegetableBox++;
        }

        notOpenInfo.scabId = notOpenStr.mid(0,length - 2);
        notOpenInfo.scellId = notOpenStr;
        notOpenInfo.errType = "1";
        boxExceptionList.append(notOpenInfo);
    }

    for(QMap<QString,qint8>::iterator it = notCheckBoxList.begin(); it != notCheckBoxList.end(); it++)
    {
        cellErrorInfo notCheckInfo;
        QString notCheckStr = it.key();
        int length = notCheckStr.size();

        if(1 == it.value())
        {
            bigBox++;
        }
        else if(2 == it.value())
        {
            middleBox++;
        }
        else if(3 == it.value())
        {
            smallBox++;
        }
        else
        {
            vegetableBox++;
        }

        notCheckInfo.scabId = notCheckStr.mid(0,length - 2);
        notCheckInfo.scellId = notCheckStr;
        notCheckInfo.errType = "5";
        boxExceptionList.append(notCheckInfo);
    }
    if(notCloseBox.size())
    {
        cellErrorInfo notCloseInfo;
        QString notCloseStr = notCloseBox.keys().first() ;
        int length = notCloseStr.size();
        QString tempErrType = "1";

        notCloseInfo.scabId = notCloseStr.mid(0,length - 2);
        notCloseInfo.scellId = notCloseStr;
        notCloseInfo.errType = "2";
        boxExceptionList.append(notCloseInfo);

        //插入到数据库
        cabinetId = notCloseStr.mid(length - 4,2).toInt(&ok,10);

        cellId = notCloseStr.mid(length - 2,2).toInt(&ok,10);

        cellType = notCloseBox.values().first();

        if(1 == cellType)
        {
            bigBox++;
        }
        else if(2 == cellType)
        {
            middleBox++;
        }
        else if(3 == cellType)
        {
            smallBox++;
        }
        else
        {
            vegetableBox++;
        }

        SCDatBase::instance()->insertOpenException(cabinetId,cellId,cellType,tempErrType);

    }


    progress.setMinimum(0);
    progress.setMaximum(0);
    progress.setValue(0);
    progress.setCancelButton(0);
    progress.setWindowFlags(Qt::FramelessWindowHint);
    progress.setLabelText(tr("数据提交中，请稍后..."));


    if(!daemonList.empty())
    {

        //播放音乐
        QString mp3file = "./music/save.mp3";
        QString vol = "15";
        SCDriver::instance()->playVoice(mp3file,vol);

        //发送消息给后台
        progress.show();
        QString delivery;
        QString opencell;
        QString getcode;
        QString celltype;
        saveDeliverysResp respone ;
        SCServerComm::instance()->saveDeliverys(M1Id,daemonList,&respone);
        if(respone.success() == "true")
        {
            for(int i = 0; i < respone.getDeliveryInfo().size(); i++)
            {
                delivery = respone.getDeliveryInfo().at(i)->deliveryId();
                opencell = respone.getDeliveryInfo().at(i)->scellId();
                getcode  = respone.getDeliveryInfo().at(i)->getCode();
                celltype = respone.getDeliveryInfo().at(i)->scellType();
                SCDatBase::instance()->deleteTakePackageCellInfo(opencell);
                SCDatBase::instance()->insertTakePackageInfo(delivery,opencell,getcode,celltype);

            }
        }


        for(int j = 0; j < daemonList.size(); j++)
        {
            if(daemonList.at(j).cellType == "1")
            {
                bigBox++;
            }
            else if(daemonList.at(j).cellType == "2")
            {
                middleBox++;
            }
            else if(daemonList.at(j).cellType == "3")
            {
                smallBox++;
            }
            else
            {
                vegetableBox++;
            }
        }


    }

    if(!boxExceptionList.empty())
    {
        reportCellErrorsResp respone;
        SCExceptioncomm::instance()->reportCellErrors(boxExceptionList,&respone);
        if(respone.success() == "false")
        {
            qDebug() << "happen exception error";
        }
    }



    byteArry.append("REFRESHCELL");
    SCDatBase::instance()->getBoxCount(cellInfo);
    SCDatBase::instance()->setBoxCount(cellInfo.bigCell - bigBox,cellInfo.middleCell - middleBox,cellInfo.smallCell - smallBox,cellInfo.vegetableCell - vegetableBox);


    progress.hide();

    notOpenBoxList.clear();
    notCloseBox.clear();
    stockList.clear();
    daemonList.clear();
    M1Id.clear();
    this->hide();

    this->okBtn->setEnabled(true);
    this->nextBtn->setEnabled(true);  

    SCMainCtrl::instance()->enterStatus(ST_HOME,byteArry);

}


void savepackagelistpage::nextBtnSlots()
{
    setBtnEnable(false);
    emit timeStop();

    QString boxType = "NO";
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out << M1Id << boxType;

    SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_MENU,data);
   // this->hide();
}


int savepackagelistpage::daemonListSize()
{
    return daemonList.size();
}

void savepackagelistpage::setNotOpenBox(QMap<QString, qint8> list)
{
    notOpenBoxList = list;
}

void savepackagelistpage::setNotCheckBox(QMap<QString, qint8> list)
{
    notCheckBoxList = list;
}

void savepackagelistpage::setNotCloseBox(QMap<QString,qint8> str)
{
    notCloseBox = str;
}

void savepackagelistpage::readInfo(QByteArray data)
{
    QString isShowList;
    packInfo daemonInfo;
    setBtnEnable(true);

    QDataStream in(&data,QIODevice::ReadOnly);
    in >> isShowList >> notOpenBoxList >> notCheckBoxList >> M1Id >> daemonInfo.packageId >> daemonInfo.mobilePhone  >> daemonInfo.cellType;
    in >> daemonInfo.cellId >> daemonInfo.isConfirm >> daemonInfo.isCheck ;


    QString tempCell = daemonInfo.cellId.right(4);

    if (isShowList == "ADD_DATA" || isShowList == "ADD_DATA_NO_Wait")
    {
        stockList.append(QString("%1").arg(++packageNumber));
        stockList.append(daemonInfo.packageId);
        stockList.append(daemonInfo.mobilePhone);
        stockList.append(tempCell.left(2));
        stockList.append(tempCell.right(2));

        daemonList.append(daemonInfo);
    }

//    if(showModel)
//    {
//        delete showModel;
//        showModel = NULL;
//    }
//    showModel = new CFormTablemodel();

    showModel->setdata(stockList);
    showModel->refrushModel();
    savetableView->setModel(showModel);
}
