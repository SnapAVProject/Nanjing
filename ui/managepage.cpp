#include <QFile>
#include <QTextStream>

#include "managepage.h"
#include "ui_managepage.h"
#include "lockdrv.h"
#include "sendimage.h"
#include "mainctrl.h"
#include "servercomm.h"
#include "db.h"
#include "configure/mconfig.h"
managepage::managepage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::managepage)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    init();
}

managepage::~managepage()
{
    delete ui;
}

void managepage::init()
{
    this->setStyleSheet("QDialog{background-image: url(:/public/images/public/beijing.png)}");

//    QLabel *portlabel = new QLabel(this);
//    portlabel->resize(100,35);
//    portlabel->move(210,53);
//    portlabel->setAlignment(Qt::AlignCenter);
//    portlabel->setStyleSheet("background-image: url(:/page/images/manage/word2.png)");

//    QLabel *serveraddrlabel = new QLabel(this);
//    serveraddrlabel->resize(160,40);
//    serveraddrlabel->move(155,126);
//    serveraddrlabel->setAlignment(Qt::AlignCenter);
//    serveraddrlabel->setStyleSheet("background-image: url(:/page/images/manage/word3.png)");

//    QLabel *md5Id = new QLabel(this);
//    md5Id->resize(134,45);
//    md5Id->move(175,199);
//    md5Id->setStyleSheet("background-image: url(:/page/images/manage/word4.png)");

    QLabel *sendimage = new QLabel(this);
    sendimage->resize(128,44);
    sendimage->move(180,50);
    sendimage->setStyleSheet("background-image: url(:/page/images/manage/word5.png)");


    QLabel *openCell = new QLabel(this);
    openCell->resize(120,40);
    openCell->move(190,130);
    openCell->setStyleSheet("background-image: url(:/page/images/setPort/word4.png)");



//    portId_lineEdit = new CKeyBoardLineEdit(this);
//    portId_lineEdit->resize(350,40);
//    portId_lineEdit->move(330,53);

//    serveraddr_lineEdit = new CKeyBoardLineEdit(this);
//    serveraddr_lineEdit->resize(350,40);
//    serveraddr_lineEdit->move(330,126);

//    md5Code_lineEdit = new CKeyBoardLineEdit(this);
//    md5Code_lineEdit->resize(350,40);
//    md5Code_lineEdit->move(330,199);
//    md5Code_lineEdit->setEchoMode(QLineEdit::Password);

    openCell_lineEdit = new CKeyBoardLineEdit(this);
    openCell_lineEdit->resize(200,40);
    openCell_lineEdit->move(330,130);
    openCell_lineEdit->setMaxLength(4);

    openBoxBtn = new QPushButton(this);
    openBoxBtn->resize(99,62);
    openBoxBtn->move(580,115);
    openBoxBtn->setFlat(true);
    openBoxBtn->setFocusPolicy(Qt::NoFocus);
    openBoxBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/take/okBtn.png)");

    sendImageBtn = new QPushButton(this);
    sendImageBtn->resize(120,47);
    sendImageBtn->move(330,40);
    sendImageBtn->setFlat(true);
    sendImageBtn->setFocusPolicy(Qt::NoFocus);
    sendImageBtn->setStyleSheet("background-color:transparent;background-image: url(:/page/images/manage/openuploadBtn.png)");
    sendStatus = true;

//    QPushButton *okbtn = new QPushButton(this);
//    okbtn->resize(182,70);
//    okbtn->move(170,494);
//    okbtn->setFlat(true);
//    okbtn->setFocusPolicy(Qt::NoFocus);
//    okbtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/okBtn.png)");


    QPushButton *cancelbtn = new QPushButton(this);
    cancelbtn->resize(182,70);
    cancelbtn->move(300,494);
    cancelbtn->setFlat(true);
    cancelbtn->setFocusPolicy(Qt::NoFocus);
    cancelbtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/exitBtn.png)");


    //设置网络ip地址
    QLabel *setIPAddrLable = new QLabel(this);
    setIPAddrLable->resize(224,67);
    setIPAddrLable->move(90,200);
    //setIPAddrLable->setText("网络通讯测试地址");
    setIPAddrLable->setStyleSheet("background-image: url(:/page/images/manage/word12.png)");

    ipAddr_lineEdit = new CKeyBoardLineEdit(this);
    ipAddr_lineEdit->resize(200,40);
    ipAddr_lineEdit->move(330,210);

    ipAddrBoxBtn = new QPushButton(this);
    ipAddrBoxBtn->resize(99,62);
    ipAddrBoxBtn->move(580,200);
    ipAddrBoxBtn->setFlat(true);
    ipAddrBoxBtn->setFocusPolicy(Qt::NoFocus);
    ipAddrBoxBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/take/okBtn.png)");


  //  connect(okbtn,SIGNAL(clicked()),this,SLOT(close()));
    connect(cancelbtn,SIGNAL(clicked()),this,SLOT(close()));
    connect(sendImageBtn,SIGNAL(clicked()),this,SLOT(isSendImage_slot()));
    connect(openBoxBtn,SIGNAL(clicked()),this,SLOT(openCell_slot()));
    connect(openCell_lineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));
    connect(ipAddrBoxBtn,SIGNAL(clicked()),this,SLOT(setIPAddr_slot()));

//    connect(portId_lineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));
//    connect(serveraddr_lineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));
//    connect(md5Code_lineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));

}

//true: 开启状态  false: 关闭状态

void managepage::isSendImage_slot()
{
    emit resetTime();

    QString uploadImage;

    if(sendStatus)
    {
        sendStatus = false;
        uploadImage = "false";

        SCSendImageThrd::instance()->setSendImage();
        SCDatBase::instance()->setUploadImages(uploadImage);
        sendImageBtn->setStyleSheet("background-color:transparent;background-image: url(:/page/images/manage/closeuploadBtn.png)");
    }
    else
    {
        sendStatus = true;
        uploadImage = "true";

        SCSendImageThrd::instance()->setSendImage();
        SCDatBase::instance()->setUploadImages(uploadImage);
        sendImageBtn->setStyleSheet("background-color:transparent;background-image: url(:/page/images/manage/openuploadBtn.png)");
    }


}

void managepage::openCell_slot()
{
    emit resetTime();
    bool ok;

    QString waitOpenBox = openCell_lineEdit->text().trimmed();
    QRegExp rxpasswd("^0\\d{3}");

    openBoxBtn->setEnabled(false);
    if (rxpasswd.exactMatch(waitOpenBox) && waitOpenBox != "0020")
    {
        qint8 cabinetId = waitOpenBox.left(2).toInt(&ok,10);
        qint8 cellId = waitOpenBox.right(2).toInt(&ok,10);
        if(lockoperation.openLock(cabinetId,cellId) != 1)
        {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("箱门打不开"));
          // QMessageBox::warning(this,tr("提示"),tr("箱门打不开"));
        }
    }
    else
    {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("输入正确的箱门"));
        // QMessageBox::warning(this,tr("提示"),tr("输入正确的箱门"));
    }
    openBoxBtn->setEnabled(true);
}

void managepage::setIPAddr_slot()
{
    QString stringtemp = ipAddr_lineEdit->text();
    if(stringtemp.isEmpty()||(!stringtemp.contains('.')))
    {
        myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("请输入正确的IP地址"));
    }
    else if(!Smconfig::instance()->writePingIP(stringtemp))
    {
        myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("设置ping网络失败"));
    }
}


void managepage::readinfo()
{

//    QString port;
//    QString serveraddr;
//    SCDatBase::instance()->getServerInfo(&serveraddr,&port);
//    QString md5Code = SCDatBase::instance()->getMdCode();
    QString uploadImage = SCDatBase::instance()->getUploadImages();

//    portId_lineEdit->setText(port);
//    serveraddr_lineEdit->setText(serveraddr);
//    md5Code_lineEdit->setText(md5Code);
//    portId_lineEdit->setFocus();
    openCell_lineEdit->setFocus();
    openCell_lineEdit->clear();

    if(uploadImage == "true")
    {
        sendImageBtn->setStyleSheet("background-color:transparent;background-image: url(:/page/images/manage/openuploadBtn.png)");
        sendStatus = true;
    }
    else
    {
        sendImageBtn->setStyleSheet("background-color:transparent;background-image: url(:/page/images/manage/closeuploadBtn.png)");
        sendStatus = false;
    }

    QString stringtemp;
    Smconfig::instance()->readPingIP(stringtemp);
    qDebug() << "managepage read ip address :" << stringtemp;
    ipAddr_lineEdit->setText(stringtemp);
}

//void managepage::okbtn_slots()
//{
//    emit resetTime();

//    QRegExp portrx("[1-9]\\d{3,4}");
//    QRegExp serveraddrrx("\\d+\\.\\d+\\.\\d+\\.\\d+");

//    QString port = portId_lineEdit->text().trimmed();
//    QString serveraddr = serveraddr_lineEdit->text().trimmed();
//    QString md5Code = md5Code_lineEdit->text().trimmed();

//    if(!portrx.exactMatch(port) || !serveraddrrx.exactMatch(serveraddr) || (md5Code.size() == 0))
//    {
//        portId_lineEdit->clear();
//        serveraddr_lineEdit->clear();

//        myMessageBox.myPrompt(QMessageBox::Critical, tr("提示"), tr("请输入正确的IP、端口、终端号和MD5密匙"));
//       // QMessageBox::critical( NULL, tr("提示"), tr("请输入正确的IP、端口、终端号和MD5密匙"));
//    }
//    else
//    {
//        if(setinfo(port,serveraddr,md5Code))
//        {
//            SCServerComm::instance()->init(serveraddr, port);
//            SCSendImageThrd::instance()->setIpPort(serveraddr,port);
//            myMessageBox.myPrompt(QMessageBox::Information,tr("提示"), tr("设置成功"));
//           // QMessageBox::information(NULL, tr("提示"), tr("设置成功"));
//        }
//    }
//}

//bool managepage::setinfo(QString port, QString serveraddr,QString md5Code)
//{
//   bool ok_s = SCDatBase::instance()->setServerInfo(serveraddr,port);
//   bool ok_m = SCDatBase::instance()->setMdCode(md5Code);

//   if(ok_s && ok_m)
//   {
//        return true;
//   }
//   else
//   {
//        return false;
//   }
//}





void managepage::closeEvent(QCloseEvent *)
{
    SCMainCtrl::instance()->enterStatus(ST_HOME, "");

}

void managepage::changeEvent(QEvent *e)
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
