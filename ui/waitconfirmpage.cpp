#include "waitconfirmpage.h"
#include "ui_waitconfirmpage.h"
#include "servercomm.h"
#include "configure/db.h"
#include "mainctrl.h"
#include <QDataStream>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

waitconfirmpage::waitconfirmpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::waitconfirmpage)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);

    initPage();

}

waitconfirmpage::~waitconfirmpage()
{
    delete ui;
}

void waitconfirmpage::initPage()
{

    this->setObjectName("QWidget");
    this->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");

    QLabel *promptLabel = new QLabel(this);
    promptLabel->resize(741,75);
    promptLabel->move(10,30);
    promptLabel->setStyleSheet("border-image: url(:/page/images/waitConfirm/word1.png)");

    QLabel *idLabel = new QLabel(this);
    idLabel->resize(149,34);
    idLabel->move(175,150);
    idLabel->setStyleSheet("border-image: url(:/page/images/waitConfirm/word2.png)");

    idLineEdit = new QLineEdit(this);
    idLineEdit->resize(256,38);
    idLineEdit->move(350,153);
    idLineEdit->setReadOnly(true);
    idLineEdit->setStyleSheet("font-family:wenquanyi; font-size:30px");

    QLabel *phoneLabel = new QLabel(this);
    phoneLabel->resize(223,34);
    phoneLabel->move(95,230);
    phoneLabel->setStyleSheet("border-image: url(:/page/images/waitConfirm/word3.png)");

    phoneLineEdit = new QLineEdit(this);
    phoneLineEdit->resize(250,40);
    phoneLineEdit->move(350,226);
    phoneLineEdit->setReadOnly(true);
    phoneLineEdit->setStyleSheet("font-family:wenquanyi; font-size:30px");

    QLabel *boxLabel = new QLabel(this);
    boxLabel->resize(78,33);
    boxLabel->move(240,299);
    boxLabel->setAlignment(Qt::AlignRight);
    boxLabel->setStyleSheet("border-image: url(:/page/images/waitConfirm/word4.png)");

    boxLineEdit = new QLineEdit(this);
    boxLineEdit->resize(250,40);
    boxLineEdit->move(350,299);
    boxLineEdit->setReadOnly(true);
    boxLineEdit->setStyleSheet("font-family:wenquanyi; font-size:30px");


    okBtn = new QPushButton(this);
    okBtn->resize(182,70);
    okBtn->move(170,494);
    okBtn->setFlat(true);
    okBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/waitConfirm/okBtn.png)");
    okBtn->setFocusPolicy(Qt::NoFocus);


    cancelBtn = new QPushButton(this);
    cancelBtn->resize(182,70);
    cancelBtn->move(453,494);
    cancelBtn->setFlat(true);
    cancelBtn->setFocusPolicy(Qt::NoFocus);
    cancelBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/waitConfirm/cancelBtn.png)");


    connect(okBtn,SIGNAL(clicked()),this,SLOT(okBtn_slot()));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtn_slot()));
    connect(&myMessageBox,SIGNAL(message_ok()),this,SLOT(startCancelPackage_slot()));

}

void waitconfirmpage::readwaitconfirminfo(QByteArray data)
{
    QString packageid;
    QString mobilephone;

    QDataStream in(&data,QIODevice::ReadOnly);
    in >> m1cardId >> packageid >> mobilephone >> cellId >> deliveryId >> isFix;
    in >> bigCellIdList >> middleCellIdList >> smallCellIdList >> vegetablesCellidList >> mTariffType >> mBalance >> personType >> blackNameList;
    in >> orderBigCells >> orderMiddleCells >> orderSmallCells >> orderVegeCells >> orderCells;

    QString showCell = cellId.right(4);
    showCell.insert(2,tr("柜"));
    showCell.append(tr("箱"));

    idLineEdit->setText(packageid);
    phoneLineEdit->setText(mobilephone);
    boxLineEdit->setText(showCell);
}

void waitconfirmpage::cancelBtn_slot()
{
    emit resetTime();

    myMessageBox.myPrompt(QMessageBox::Warning,tr("提示确认"),tr("您确定此箱门中没有存放派件？"));
}


void waitconfirmpage::startCancelPackage_slot()
{
    okBtn->setEnabled(false);
    cancelBtn->setEnabled(false);
    idLineEdit->clear();
    phoneLineEdit->clear();
    boxLineEdit->clear();

    QString type = "2";
    updatePackageStatusResp respone ;
    SCServerComm::instance()->updatePackageStatus(deliveryId,type,&respone);


    if(respone.success() == "true")
    {
        recvBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("派件已取消。"));

        if((mTariffType.toInt() == 2 || mTariffType.toInt() == 4) && ((!isFix && mBalance.toDouble() <= 0)|| (isFix && mBalance.toDouble() < 0)))
        {
            QByteArray byteArry;
            byteArry.append("REFRESHCELL");
            cancelDebtUpdateBox(respone.scellType());
           // myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("账户余额不足，请先充值。"));
            SCMainCtrl::instance()->enterStatus(ST_HOME,byteArry);
        }
        else
        {          
            cancelNoDebtUpdateBox(respone.scellType());

            QByteArray     outData;
            QDataStream out(&outData,QIODevice::WriteOnly);
            out << m1cardId << personType << bigCellIdList << middleCellIdList << smallCellIdList << vegetablesCellidList << mTariffType << mBalance << blackNameList\
                << orderBigCells << orderMiddleCells << orderSmallCells << orderVegeCells << orderCells;

            SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_MENU,outData);  //存件
        }
    }
    else if(respone.success() == "false")
    {
        myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("发送错误,请重新点击。"));
    }
    else
    {
        myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"));
    }


    okBtn->setEnabled(true);
    cancelBtn->setEnabled(true);
}


int waitconfirmpage::cancelNoDebtUpdateBox(QString cellType)
{
    int type = 0;
    if(cellType.toInt() == 1)
    {
        bigCellIdList.append(cellId.right(4));
        type = 1;
    }
    else if(cellType.toInt() == 2)
    {
        middleCellIdList.append(cellId.right(4));
        type = 2;
    }
    else if(cellType.toInt() == 3)
    {
        smallCellIdList.append(cellId.right(4));
        type = 3;
    }
    else
    {
        vegetablesCellidList.append(cellId.right(4));
        type = 4;
    }

    return type;
}


void waitconfirmpage::cancelDebtUpdateBox(QString cellType)
{
    CellInfoDesc_t cellInfo;
    SCDatBase::instance()->getBoxCount(cellInfo);

    int type = cancelNoDebtUpdateBox(cellType);
    if(1 == type)
    {
        cellInfo.bigCell = cellInfo.bigCell + 1;
    }
    else if(2 == type)
    {
        cellInfo.middleCell = cellInfo.middleCell + 1;
    }
    else if(3 == type)
    {
        cellInfo.smallCell = cellInfo.smallCell + 1;
    }
    else if(4 == type)
    {
        cellInfo.vegetableCell = cellInfo.vegetableCell + 1;
    }

    SCDatBase::instance()->setBoxCount(cellInfo.bigCell,cellInfo.middleCell,cellInfo.smallCell,cellInfo.vegetableCell);
}

void waitconfirmpage::okBtn_slot()
{
    emit resetTime();
    okBtn->setEnabled(false);
    cancelBtn->setEnabled(false);

    QString type = "1";
    updatePackageStatusResp respone ;
    SCServerComm::instance()->updatePackageStatus(deliveryId,type,&respone);

    if(respone.success() == "true")
    {
        idLineEdit->clear();
        phoneLineEdit->clear();
        boxLineEdit->clear();

        for(int i = 0; i < respone.getDeliveryInfo().size(); i++)
        {
            QString delivery = respone.getDeliveryInfo().at(i)->deliveryId();
            QString opencell = respone.getDeliveryInfo().at(i)->scellId();
            QString getcode  = respone.getDeliveryInfo().at(i)->getCode();
            QString celltype = respone.getDeliveryInfo().at(i)->scellType();
            SCDatBase::instance()->insertTakePackageInfo(delivery,opencell,getcode,celltype);

        }

        recvBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("派件已确认。"));

        if((mTariffType.toInt() == 2 || mTariffType.toInt() == 4) && ((!isFix && mBalance.toDouble() <= 0)|| (isFix && mBalance.toDouble() < 0)))
        {
          //  myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("账户余额不足，请先充值。"));
            SCMainCtrl::instance()->enterStatus(ST_HOME,"");
        }
        else
        {
            QByteArray     outData;
            QDataStream out(&outData,QIODevice::WriteOnly);
            out << m1cardId << personType << bigCellIdList << middleCellIdList << smallCellIdList << vegetablesCellidList << mTariffType << mBalance << blackNameList\
                << orderBigCells << orderMiddleCells << orderSmallCells << orderVegeCells << orderCells;

            SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_MENU,outData); //存件
        }

    }
    else if(respone.success() == "false")
    {
        myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("发送错误,请重新点击"));
    }
    else
    {
        myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"));
    }

     okBtn->setEnabled(true);
     cancelBtn->setEnabled(true);

}

void waitconfirmpage::changeEvent(QEvent *e)
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
