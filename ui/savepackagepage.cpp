#include "savepackagepage.h"
#include "ui_savepackagepage.h"

#include "driver.h"
#include "mainctrl.h"
#include <QBoxLayout>
#include <QToolButton>
#include <QStackedLayout>
#include <QLabel>
#include <QPushButton>
#include <QDataStream>
#include <QPalette>
#include "configure/db.h"
#include "lockdrv.h"
#include <QDebug>

//***************************************************************************
/**file		savepackagepage.cpp
* brief		存件
* author	hjzhang@163.com	2013/9/26
* version	1.3
*/
//***************************************************************************

savepackagepage::savepackagepage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::savepackagepage)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    bigstatus = true;
    middlestatus = true;
    smallstatus = true;
    Cur_Status = NO_ST;

    initPackageInfo();


}

savepackagepage::~savepackagepage()
{
    delete ui;
}


void savepackagepage::readdisplayinfo(QByteArray param)
{
    bigstatus = true;
    middlestatus = true;
    smallstatus = true;
    Cur_Status = NO_ST;

    this->bigCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
    this->middleCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
    this->smallCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
    this->vegetableCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));

    setEableEdit(false); //设置编辑框不可用

    initlist(param);

    setEableEdit(true);
    smallBoxBtn->setEnabled(true);
    middleBoxBtn->setEnabled(true);
    bigBoxBtn->setEnabled(true);
    vegetableBtn->setEnabled(true);


    updatebox();  //判断箱子数量

   // loadcodeScan();

}



/***************************************************************

        函数名称：initPackageInfo()
        函数功能：初始话界面
        函数参数： 无
        返回值：无

***************************************************************/
void savepackagepage::initPackageInfo()
{
    this->setObjectName("QWidget");
    this->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");

    balanceLabel = new QLabel(this);
    balanceLabel->resize(250,40);
    balanceLabel->move(550,10);
    balanceLabel->setAlignment(Qt::AlignRight);
    balanceLabel->setStyleSheet("font-family:wenquanyi; font-size:24px;color:rgb(255,255,255)");

    QLabel *idLabel = new QLabel(this);
    idLabel->resize(312,30);
    idLabel->move(30,55);
    idLabel->setStyleSheet("border-image: url(:/page/images/saveStart/word2.png)");

    barCodeLineEdit = new CKeyBoardLineEdit(this);
    barCodeLineEdit->installEventFilter(this);
    barCodeLineEdit->resize(270,40);
    barCodeLineEdit->move(350,53);
    barCodeLineEdit->setPlaceholderText(tr("请扫描"));


    QLabel *phoneLabel = new QLabel(this);
    phoneLabel->resize(312,30);
    phoneLabel->move(30,130);
    phoneLabel->setStyleSheet("border-image: url(:/page/images/saveStart/word3.png)");


    phoneLineEdit = new CKeyBoardLineEdit(this);
    phoneLineEdit->installEventFilter(this);
    phoneLineEdit->setMaxLength(11);
    phoneLineEdit->resize(270,40);
    phoneLineEdit->move(350,126);


    QLabel *againPhoneLabel = new QLabel(this);

    againPhoneLabel->resize(222,30);
    againPhoneLabel->move(118,199);
    againPhoneLabel->setStyleSheet("background-image: url(:/page/images/saveStart/word4.png)");


    againPhoneLineEdit = new CKeyBoardLineEdit(this);
    againPhoneLineEdit->installEventFilter(this);
    againPhoneLineEdit->setMaxLength(11);
    againPhoneLineEdit->resize(270,40);
    againPhoneLineEdit->move(350,199);


    okBtn = new QPushButton(this);
    okBtn->setFlat(true);
    okBtn->resize(182,70);
    okBtn->move(170,494);
    okBtn->setFocusPolicy(Qt::NoFocus);
    okBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/okBtn.png)");

/*
    this->setAutoFillBackground(true);
    QPalette palette;
    QPixmap  pixmap=QPixmap("./images/images/btn_bg.png");
    palette.setBrush(QPalette::Button,QBrush(pixmap));
    okBtn->setPalette(palette);
*/
    cancelBtn = new QPushButton(this);
    cancelBtn->setFlat(true);
    cancelBtn->resize(182,70);
    cancelBtn->move(453,494);
    cancelBtn->setFocusPolicy(Qt::NoFocus);
    cancelBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/exitBtn.png)");


    //小号
    smallBoxBtn = new QToolButton(this);
    smallBoxBtn->resize(185,150);
    smallBoxBtn->move(5,300);
    smallBoxBtn->setStyleSheet("background-color:transparent");
    smallBoxBtn->setFocusPolicy(Qt::NoFocus);


    smallBoxLab = new QLabel(smallBoxBtn);    //显示箱子剩余量
    smallBoxLab->resize(60,60);
    smallBoxLab->move(0,95);
    smallBoxLab->setAlignment(Qt::AlignCenter);
    smallBoxLab->setStyleSheet("font-family:wenquanyi; font-size:40px; color:rgb(255, 170, 0)");

    QLabel *smallTextLab = new QLabel(smallBoxBtn);   //小号箱的图片
    smallTextLab->move(50,0);
    smallTextLab->setPixmap(QPixmap(":/page/images/saveStart/small.png"));

    smallCheckedLab = new QLabel(smallBoxBtn);  //是否选中图片
    smallCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
    smallCheckedLab->move(152,110);


    //中号
    middleBoxBtn = new QToolButton(this);
    middleBoxBtn->resize(185,150);
    middleBoxBtn->move(210,300);
    middleBoxBtn->setStyleSheet("background-color:transparent");
    middleBoxBtn->setFocusPolicy(Qt::NoFocus);


    middleBoxLab = new QLabel(middleBoxBtn);
    middleBoxLab->resize(60,60);
    middleBoxLab->move(0,95);
    middleBoxLab->setAlignment(Qt::AlignCenter);
    middleBoxLab->setStyleSheet("font-family:wenquanyi; font-size:40px; color:rgb(255, 170, 0)");

    QLabel *middleTextLab = new QLabel(middleBoxBtn);
    middleTextLab->move(50,0);
    middleTextLab->setPixmap(QPixmap(":/page/images/saveStart/middle.png"));

    middleCheckedLab = new QLabel(middleBoxBtn);
    middleCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
    middleCheckedLab->move(152,110);

    //大号
    bigBoxBtn = new QToolButton(this);
    bigBoxBtn->resize(185,150);
    bigBoxBtn->move(405,300);
    bigBoxBtn->setStyleSheet("background-color:transparent");


    bigBoxLab = new QLabel(bigBoxBtn);
    bigBoxLab->resize(60,60);
    bigBoxLab->move(0,95);
    bigBoxLab->setAlignment(Qt::AlignCenter);
    bigBoxLab->setStyleSheet("font-family:wenquanyi; font-size:40px; color:rgb(255, 170, 0)");
    bigBoxBtn->setFocusPolicy(Qt::NoFocus);

    QLabel *bigTextLab = new QLabel(bigBoxBtn);
    bigTextLab->move(50,0);
    bigTextLab->setPixmap(QPixmap(":/page/images/saveStart/big.png"));

    bigCheckedLab = new QLabel(bigBoxBtn);
    bigCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
    bigCheckedLab->move(152,110);

    //蔬菜
    vegetableBtn = new QToolButton(this);
    vegetableBtn->resize(185,150);
    vegetableBtn->move(600,300);
    vegetableBtn->setStyleSheet("background-color:transparent");
    vegetableBtn->setFocusPolicy(Qt::NoFocus);

    vegetableLab = new QLabel(vegetableBtn);
    vegetableLab->resize(60,60);
    vegetableLab->move(0,95);
    vegetableLab->setAlignment(Qt::AlignCenter);
    vegetableLab->setStyleSheet("font-family:wenquanyi; font-size:40px; color:rgb(255, 170, 0)");

    QLabel *vegetableTextLab = new QLabel(vegetableBtn);
    vegetableTextLab->move(50,0);
    vegetableTextLab->setPixmap(QPixmap(":/page/images/saveStart/vegetable.png"));

    vegetableCheckedLab = new QLabel(vegetableBtn);
    vegetableCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
    vegetableCheckedLab->move(152,110);



    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(bigBoxBtn,0);
    buttonGroup->addButton(middleBoxBtn,1);
    buttonGroup->addButton(smallBoxBtn,2);
    buttonGroup->addButton(vegetableBtn,3);
    buttonGroup->setExclusive(true);  //互斥


    connect(okBtn,SIGNAL(clicked()),this,SLOT(okslots()));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancelslots()));
    connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonclickslots(int)));

    connect(barCodeLineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));
    connect(phoneLineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));
    connect(againPhoneLineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));


}



void savepackagepage::btnconfirminfo()
{  
    //保存发送给后台的信息
    daemonInfo.packageId = this->barCodeLineEdit->text().trimmed();
    daemonInfo.mobilePhone = this->phoneLineEdit->text().trimmed();

    //保存显示列表的信息
   // stockinfo.Id = QString("%1").arg(stockList.length()+1);
  //  stockInfo.expressId = this->lineEdit->text();
  //  stockInfo.mobilePhone = this->phoneLineEdit->text();

    //减去占用的箱子并记录进发给后台数据中
    if(this->Cur_Status == BIG_ST)
    {
  //      sbox = bigCellList.last();
        daemonInfo.cellType = "1";
        daemonInfo.cellId   = terminal + bigCellList.last();
  //      qDebug() << "大箱";
        this->bigCellList.removeLast();
    }
    else if(this->Cur_Status == MID_ST)
    {
  //      sbox = middleCellList.last();
        daemonInfo.cellType = "2";
        daemonInfo.cellId   = terminal + middleCellList.last();
        this->middleCellList.removeLast();
    }
    else if(this->Cur_Status == SMA_ST)
    {
   //     sbox = smallCellList.last();
        daemonInfo.cellType = "3";
        daemonInfo.cellId   = terminal + smallCellList.last();
        this->smallCellList.removeLast();
    }
    else
    {
        daemonInfo.cellType = "4";
        daemonInfo.cellId   = terminal + vegetableCellList.last();
        this->vegetableCellList.removeLast();
    }


    daemonInfo.isConfirm = "yes";  // 默认是确认件
    daemonInfo.isCheck = "3";      // 1有物品、2无物品 3无物品检测条(默认）

}






/***************************************************************

        函数名称：updatebox()
        函数功能：处理箱子剩余情况
        函数参数： 无
        返回值：无

***************************************************************/
void savepackagepage::updatebox()
{
    bigBoxLab->setText(QString("%1").arg(bigCellList.size()));
    middleBoxLab->setText(QString("%1").arg(middleCellList.size()));
    smallBoxLab->setText(QString("%1").arg(smallCellList.size()));
    vegetableLab->setText(QString("%1").arg(vegetableCellList.size()));



    QString bigbox = bigBoxLab->text();
    QString middlebox = middleBoxLab->text();
    QString smallbox = smallBoxLab->text();
    QString vegetablebox = vegetableLab->text();

    this->bigstatus = true;
    this->middlestatus = true;
    this->smallstatus = true;
    this->vegetablestatus = true;


    if("0" == bigbox)
    {
        this->bigstatus = false;   //不能选中,告知大箱为false
        this->bigCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
    }
    if("0" == middlebox)
    {
        this->middlestatus = false;
        this->middleCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
    }
    if("0" == smallbox)
    {
        this->smallstatus = false;
        this->smallCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
    }
    if("0" == vegetablebox)
    {
        this->vegetablestatus = false;
        this->vegetableCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
    }


}


/****************************************************************
**
*	函数名称：	buttonclickslots
*	函数功能：	选择箱体
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void savepackagepage::buttonclickslots(int buttonid)
{
    emit resetTime();
  //  qDebug() << "buttonid:" << buttonid << "bigstatus:" << bigstatus;
    if(0 == buttonid && bigstatus)
    {
        this->bigCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/yes.png"));
        this->middleCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->smallCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->vegetableCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->Cur_Status = BIG_ST;
    }
    if(1 == buttonid && middlestatus)
    {
        this->bigCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->middleCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/yes.png"));
        this->smallCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->vegetableCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->Cur_Status = MID_ST;
    }
    if(2 == buttonid && smallstatus)
    {
        this->bigCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->middleCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->smallCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/yes.png"));
        this->vegetableCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->Cur_Status = SMA_ST;
    }
    if(3 == buttonid && vegetablestatus)
    {
        this->bigCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->middleCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->smallCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->vegetableCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/yes.png"));
        this->Cur_Status = VEG_ST;
    }
}


/****************************************************************
**
*	函数名称：	cancelslots
*	函数功能：	填写寄存信息判断
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void savepackagepage::cancelslots()
{
    enterboxstatus(NO_ST);

    okBtn->setEnabled(false);
    cancelBtn->setEnabled(false);
    //closeEventFilter();

    QByteArray data;
    QString isShowList = "NO_ADD_DATA_NO_WAITCLOSE_BOX"; //不需添加没有未关闭箱子
    QDataStream out(&data,QIODevice::WriteOnly);
    out << isShowList << notOpenBoxList << notCheckBoxList << M1Id;

    SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_LIST, data);

}



/****************************************************************
**
*	函数名称：	okslots
*	函数功能：	填写寄存信息判断
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void savepackagepage::okslots()
{
    emit resetTime();
    QByteArray data;
    QStringList* list;
    QString info;

    if (inputCheck())
    {
        okBtn->setEnabled(false);
        cancelBtn->setEnabled(false);

        setEableEdit(false);

        if (Cur_Status == BIG_ST)
        {
            list = &bigCellList;
            info = "大号箱门打不开，重新选择箱门大小!";
        }
        else if(Cur_Status == MID_ST)
        {
            list = &middleCellList;
            info = "中号箱门打不开，重新选择箱门大小!";
        }
        else if (Cur_Status == SMA_ST)
        {
            list = &smallCellList;
            info = "小号箱门打不开，重新选择箱门大小!";
        }
        else if(Cur_Status == VEG_ST)
        {
            list = &vegetableCellList;
            info = "保鲜箱门打不开，请重新选择箱门大小！";
        }
        else
        {
            qDebug() << "Cur_Status is error";
            return ;
        }

        if(openSelectLocks(list))   //打开箱子
        {
            btnconfirminfo();    //记录当前要显示信息与要发后台的存件信息
            packdata(data);

            SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_WAIT, data); //存件等待确认

        }
        else
        {
            okBtn->setEnabled(true);
            cancelBtn->setEnabled(true);

            enterboxstatus(NO_ST);
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"), info);
           // QMessageBox::warning(this,tr("提示"), info);
        }
        setEableEdit(true);
    }
}


bool savepackagepage::inputCheck()
{
    QRegExp rxcellphone("^1\\d{10}");
    bool ret = true;
    QString info;

    //下面判断的（后期用到）
    if(barCodeLineEdit->text() == "")
    {
        info = "请扫入快递单号。";
        ret = false;
    }
    else if(!rxcellphone.exactMatch(this->phoneLineEdit->text().trimmed()))
    {
      //  this->phoneLineEdit->clear();
       // this->againPhoneLineEdit->clear();
        ret = false;
        info = "手机号码输入有误！请输入正确号码。";
    }
    else if (this->phoneLineEdit->text().trimmed() != this->againPhoneLineEdit->text().trimmed())
    {
      //  this->phoneLineEdit->clear();
     //   this->againPhoneLineEdit->clear();
        ret = false;
        info = "手机号码输入不一致，请重新确认。";
    }
    else if(Cur_Status == NO_ST)  //选中箱号
    {
        ret = false;
        info = "选择箱子类型。";
    }

    //屏蔽手机专用 start
    foreach(QString tempstr,blackNameList)
    {
        if(tempstr == this->phoneLineEdit->text())
        {
            qDebug() << "屏蔽收件人手机号码" << tempstr << "输入收件人手机号码：" << this->phoneLineEdit->text();
            ret = false;
            info = "此收件人要求包裹不放入自助包裹站，请快递员直接派送！";
        }
    }
    //end

    if (!ret)
    {
         myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"), info);
        // QMessageBox::warning(this,tr("提示"),info);
    }

    return ret;
}

/****************************************************************
**
*	函数名称：	loadcodeScan
*	函数功能：	扫描条形码
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
bool savepackagepage::loadcodeScan()
{
    //播放音乐
    QString mp3file = "./music/scan.mp3";
    QString vol = "15";
    SCDriver::instance()->playVoice(mp3file,vol);

    barCodeLineEdit->setFocus();

    bool ok = false;
    QByteArray data;
    QString barcode;
    ok = SCDriver::instance()->codeScan(data);

    if(ok && barCodeLineEdit->hasFocus())
    {
        barcode = QString(data);
        this->barCodeLineEdit->setText(barcode);
        this->focusNextChild();
        return true;
    }
  //  qDebug() << "loadcodeScan is error";

    return false;
}





/****************************************************************
**
*	函数名称：	openSelectLocks
*	函数功能：	选择打开的箱子
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/

bool savepackagepage:: openSelectLocks(QStringList* lockList)
{
    bool ok = false;
    bool ret = false;
    if (!lockList->size())
        return false;


    while(lockList->size())
    {
        QString curLockString = lockList->last();
        int boxLength = curLockString.length();
        qint8 cabinetId = curLockString.mid(boxLength - 4,2).toInt(&ok,10);
        qint8 cellId = curLockString.mid(boxLength - 2,2).toInt(&ok,10);

        if(SCDriver::instance()->readCheckGoods())
        {
            if(lockoperation.queryGoods(cabinetId,cellId) == 0)
            {
                emit resetTime();
                QString notCheckCell = terminal + lockList->last();
                notCheckBoxList.insert(notCheckCell,this->Cur_Status);
                lockList->removeLast();
                updatebox();
                continue;
            }
        }

        if (lockoperation.openLock(cabinetId,cellId) == 1)
        {
            ret = true;
            break;
        }
        else
        {
            emit resetTime();
            QString notOpenCell = terminal + lockList->last();
            notOpenBoxList.insert(notOpenCell,this->Cur_Status);
            lockList->removeLast();
            updatebox();

        }
    }

    return ret;
}




/****************************************************************
**
*	函数名称：	enterboxstatus
*	函数功能：	刷新当前状态
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void savepackagepage::enterboxstatus(BTN_STATUS status)
{
    this->Cur_Status = status;

    if(status == BIG_ST)
    {
        this->bigCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/yes.png"));
        this->middleCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->smallCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->vegetableCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
    }
    else if(status == MID_ST)
    {
        this->bigCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->middleCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/yes.png"));
        this->smallCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->vegetableCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
    }
    else if(status == SMA_ST)
    {
        this->bigCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->middleCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->smallCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/yes.png"));
        this->vegetableCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
    }
    else if(status == VEG_ST)
    {
        this->bigCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->middleCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->smallCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->vegetableCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/yes.png"));
    }
    else
    {
        this->bigCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->middleCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->smallCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));
        this->vegetableCheckedLab->setPixmap(QPixmap(":/page/images/saveStart/no.png"));

    }
}


void savepackagepage::packdata(QByteArray &data)
{
    QString currentBox;
    if(this->Cur_Status == BIG_ST)
    {
        currentBox = "BG";
    }
    else if(this->Cur_Status == MID_ST)
    {
        currentBox = "MI";
    }
    else if(this->Cur_Status == SMA_ST)
    {
        currentBox = "SM";
    }
    else
    {
        currentBox = "VG";
    }
    QDataStream out(&data,QIODevice::WriteOnly);
    out << M1Id << daemonInfo.packageId << daemonInfo.mobilePhone  << daemonInfo.cellType;
    out << daemonInfo.cellId << daemonInfo.isConfirm << daemonInfo.isCheck << currentBox << notOpenBoxList << notCheckBoxList;

}


/****************************************************************
**
*	函数名称：	enterboxstatus
*	函数功能：	刷新当前状态
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void savepackagepage::setEableEdit(bool status)
{
    if(status)
    {      
        barCodeLineEdit->setReadOnly(false);
        phoneLineEdit->setReadOnly(false);
        againPhoneLineEdit->setReadOnly(false);
        smallBoxBtn->setEnabled(true);
        middleBoxBtn->setEnabled(true);
        bigBoxBtn->setEnabled(true);
        vegetableBtn->setEnabled(true);
    }
    else
    {
        barCodeLineEdit->setReadOnly(true);
        phoneLineEdit->setReadOnly(true);
        againPhoneLineEdit->setReadOnly(true);
        smallBoxBtn->setEnabled(false);
        middleBoxBtn->setEnabled(false);
        bigBoxBtn->setEnabled(false);
        vegetableBtn->setEnabled(false);
    }
}


bool savepackagepage::eventFilter(QObject *obj, QEvent *event)
{
    bool ok = false;
    if(obj == barCodeLineEdit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
  //          qDebug() << "start codescan";
            ok = loadcodeScan();
            if(ok)
            {
//                qDebug() << "codeScan is recv";
            }
        }
    }
    if(obj == phoneLineEdit || obj == againPhoneLineEdit)
    {
        QLineEdit *myobj = static_cast<QLineEdit *>(obj);
        if(event->type() == QEvent::FocusOut)
        {
            myobj->setEchoMode(QLineEdit::PasswordEchoOnEdit);
            if(obj == againPhoneLineEdit && (!phoneLineEdit->hasFocus()))
            {
                this->setFocus();
            }
        }
        else if(event->type() == QEvent::FocusIn)
        {         
            myobj->setEchoMode(QLineEdit::Normal);
        }


    }

     return QDialog::eventFilter(obj,event);
}






/****************************************************************
**
*	函数名称：	initlist
*	函数功能：	读出箱体信息
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void savepackagepage::initlist(QByteArray param)
{
    QString isLong;
 //   QStringList vegetablelist;
    QString tariffType;
    QString balance;
    QDataStream in(&param,QIODevice::ReadOnly);
    in >> M1Id >> isLong;

//    qDebug() << "M1Id:" << M1Id << "isLong:" << isLong;
    if(isLong == "KD" || isLong == "SC")
    {
        terminal.clear();
        notOpenBoxList.clear();
        notCheckBoxList.clear();

        barCodeLineEdit->clear();
        phoneLineEdit->clear();
        againPhoneLineEdit->clear();

        bigCellList.clear();
        middleCellList.clear();
        smallCellList.clear();

        blackNameList.clear();

        vegetableCellList.clear();
        orderBigCellList.clear();
        orderMiddleCellList.clear();
        orderSmallCellList.clear();
        orderVegetableCellList.clear();
        orderCellInfos.clear();

        updatebox();

        terminal = SCDatBase::instance()->getTerminalId();

//        QString terminal = SCDatBase::instance()->getTerminalId();
//        getCellIdsByTerminalIdResp respone;
//        SCServerComm::instance()->getCellIdsByTerminalId(terminal,&respone);
//        bigCellList = respone.bigCellIdList();
//        middleCellList = respone.middleCellIdList();
//        smallCellList = respone.smallCellIdList();
//        vegetablelist = respone.vegetablesCellidList();


        in >> bigCellList >> middleCellList >> smallCellList >> vegetableCellList >> tariffType >> balance >> blackNameList;
        in >> orderBigCellList >> orderMiddleCellList >> orderSmallCellList >> orderVegetableCellList >> orderCellInfos;



        if(tariffType.toInt() == 2 || tariffType.toInt() == 4)
        {
            balanceLabel->setText(QString("帐户余额：%1元").arg(balance));
            balanceLabel->show();
        }
        else
        {
            balanceLabel->hide();
        }

   //     middleCellList = vegetablelist + middleCellList;

        filterCell();

        SCDatBase::instance()->setBoxCount(bigCellList.size(),middleCellList.size(),smallCellList.size(),vegetableCellList.size());

        checkIsFixCellInfo();
        if(isLong == "KD")
        {
            vegetableCellList.clear();
        }
       /*
        bigCellList << "1203" << "0302";
        middleCellList << "0907" << "0507";
        smallCellList << "0405";
        M1Id = "3456";
        */
    }
    else if(isLong == "BG")
    {
        bigCellList.append(daemonInfo.cellId.right(4));
        this->setFocus();

    }
    else if(isLong == "MI")
    {
        middleCellList.append(daemonInfo.cellId.right(4));
        this->setFocus();
    }
    else if(isLong == "SM")
    {
        smallCellList.append(daemonInfo.cellId.right(4));
        this->setFocus();

    }
    else if(isLong == "VG")
    {
        vegetableCellList.append(daemonInfo.cellId.right(4));
        this->setFocus();
    }
    else
    {
        //继续存件
        barCodeLineEdit->clear();
        phoneLineEdit->clear();
        againPhoneLineEdit->clear();
    }

     clearInfo();



/*
    smallCellList << "0807";
    bigCellList << "0906" << "0708";
    middlecelllist << "0204" << "0407";
    smallcelllist << "0807";
    this->bigboxlab->setText(QString("%1").arg(bigcelllist.size()));
    this->middleboxlab->setText(QString("%1").arg(middlecelllist.size()));
    this->smallboxlab->setText(QString("%1").arg(smallcelllist.size()));
*/
}



/****************************************************************
**
*	函数名称：	filterCell
*	函数功能：	过滤箱体信息
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/

void savepackagepage::filterCell()
{
    int k;
    QList<ServerDesc_t> serverlist;
    SCDatBase::instance()->queryServerException(serverlist);

    QList<CellDesc_t> celllist;
    SCDatBase::instance()->queryOpenException(celllist);

    for (int i = 0; i < serverlist.size(); i++)
    {
        if(serverlist.at(i).errType == "1")
        {
            //exitDoor: 1,000XXXXXXXXX|2,000XXXXXXXXX|
            QString exitDoor = serverlist.at(i).cellId;
            QStringList exitDoorList = exitDoor.split("|");
            for(int j = 0; j < exitDoorList.size(); j++)
            {
                QString oneExitDoor = exitDoorList.at(j);
                QStringList oneExitDoorList = oneExitDoor.split(",");

                if(oneExitDoorList.first() == "1")
                {
                    for(k = 0; k < bigCellList.size(); k++)
                    {
                        if(bigCellList.at(k) == oneExitDoorList.last().right(4))
                        {
                            bigCellList.removeAt(k);
                            break;
                        }
                    }
                }
                else if(oneExitDoorList.first() == "2")
                {
                    for(k = 0; k < middleCellList.size(); k++)
                    {
                        if(middleCellList.at(k) == oneExitDoorList.last().right(4))
                        {
                            middleCellList.removeAt(k);
                            break;
                        }
                    }
                }
                else if(oneExitDoorList.first() == "3")
                {
                    for(k = 0; k < smallCellList.size(); k++)
                    {
                        if(smallCellList.at(k) == oneExitDoorList.last().right(4))
                        {
                            smallCellList.removeAt(k);
                            break;
                        }
                    }
                }
                else if(oneExitDoorList.first() == "4")
                {
                    for(k = 0; k < vegetableCellList.size(); k++)
                    {
                        if(vegetableCellList.at(k) == oneExitDoorList.last().right(4))
                        {
                            vegetableCellList.removeAt(k);
                            break;
                        }
                    }
                }
            }
        }
    }


    for(int m = 0; m < celllist.size(); m++)
    {
        if(celllist.at(m).errType == "2")
        {
            QString cabinet = QString::number(celllist.at(m).cabinetId).sprintf("%02d",celllist.at(m).cabinetId);
            QString cell = QString::number(celllist.at(m).cellId).sprintf("%02d",celllist.at(m).cellId);
            QString tempCell = cabinet + cell;
            for(k = 0;k < bigCellList.size() ; k++)
            {
                if(bigCellList.at(k) == tempCell)
                {
                    bigCellList.removeAt(k);
                    break;
                }
            }

            for(k = 0;k < middleCellList.size() ; k++)
            {
                if(middleCellList.at(k) == tempCell)
                {
                    middleCellList.removeAt(k);
                    break;
                }
            }

            for(k = 0;k < smallCellList.size() ; k++)
            {
                if(smallCellList.at(k) == tempCell)
                {
                    smallCellList.removeAt(k);
                    break;
                }
            }

            for(k = 0;k < vegetableCellList.size() ; k++)
            {
                if(vegetableCellList.at(k) == tempCell)
                {
                    vegetableCellList.removeAt(k);
                    break;
                }
            }
        }
    }

}

void savepackagepage::checkIsFixCellInfo()
{
    if(!orderCellInfos.isEmpty())
    {
        bigCellList << orderBigCellList;
        middleCellList << orderMiddleCellList;
        smallCellList << orderSmallCellList;
        vegetableCellList << orderVegetableCellList;
        for(int i = 0; i < orderCellInfos.size(); i++)
        {
            bigCellList.removeOne(orderCellInfos.at(i));
            middleCellList.removeOne(orderCellInfos.at(i));
            smallCellList.removeOne(orderCellInfos.at(i));
            vegetableCellList.removeOne(orderCellInfos.at(i));
        }

    }
}

void savepackagepage::clearInfo()
{
    daemonInfo.packageId.clear();
    daemonInfo.mobilePhone.clear();
    daemonInfo.cellType.clear();
    daemonInfo.cellId.clear();
    daemonInfo.isConfirm.clear();
    daemonInfo.isCheck.clear();

}

void savepackagepage::setBtnEable(bool status)
{
    this->okBtn->setEnabled(status);
    this->cancelBtn->setEnabled(status);
}



void savepackagepage::changeEvent(QEvent *e)
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
