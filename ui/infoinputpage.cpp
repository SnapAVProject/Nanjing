#include "infoinputpage.h"
#include "ui_infoinputpage.h"
#include "mainctrl.h"
#include "db.h"
#include <QLabel>
#include <QListView>
#include <QPushButton>
#include <QTextBrowser>
#include <QCheckBox>
#include <QScrollBar>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDebug>

infoinputpage::infoinputpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::infoinputpage)
{
    ui->setupUi(this);


    stack = new QStackedLayout(this);

    initStatePage();
    initPage();

    this->resize(800,600);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setLayout(stack);

}

infoinputpage::~infoinputpage()
{
    delete ui;
}


/****************************************************************
**
*	函数名称：	initStatePage
*	函数功能：	初始化注册声明界面布局
* 	函数参数：	无
* 	返回值  :    无
*
****************************************************************/
void infoinputpage::initStatePage()
{
    QWidget *statePage = new QWidget(this);
    statePage->setObjectName("QWidget");
    statePage->setStyleSheet("#QWidget{background-image: url(:/page/images/info/bg.png)}");

    QString imgPath = QString(":/page/images/info/statement.png");
    imgPath = QString("<img src=\"%1\"/>").arg(imgPath);

    textContent = new QGraphicsView(statePage);
    textContent->resize(800,380);
    textContent->move(0,0);

    QGraphicsScene *scene = new QGraphicsScene(statePage);
    scene->setSceneRect(0,0,800,550);

    QGraphicsPixmapItem *img = new QGraphicsPixmapItem();
    img->setPixmap(QPixmap(":/page/images/info/statement.png"));
    scene->addItem(img);
    textContent->setScene(scene);
    textContent->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textContent->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    textContent->setStyleSheet("background-color:transparent");
    textContent->verticalScrollBar()->setValue(1);

    checkBox = new QCheckBox(statePage);
    checkBox->resize(120,40);
    checkBox->move(250,400);
    checkBox->setFocusPolicy(Qt::NoFocus);
    checkBox->setStyleSheet("QCheckBox::indicator:nochecked {image: url(:/page/images/info/agree.png)}QCheckBox::indicator:checked {image: url(:/page/images/info/agree_yes.png)}");

    opposeCheckBox = new QCheckBox(statePage);
    opposeCheckBox->resize(120,40);
    opposeCheckBox->move(450,400);
    opposeCheckBox->setFocusPolicy(Qt::NoFocus);
    opposeCheckBox->setCheckState(Qt::Checked);
    opposeCheckBox->setStyleSheet("QCheckBox::indicator:nochecked {image: url(:/page/images/info/oppose.png)}QCheckBox::indicator:checked {image: url(:/page/images/info/oppose_yes.png)}");


    QButtonGroup *buttonGroup = new QButtonGroup(statePage);
    buttonGroup->addButton(checkBox,0);
    buttonGroup->addButton(opposeCheckBox,1);
    buttonGroup->setExclusive(true);  //互斥

    stateOkBtn = new QPushButton(statePage);
    stateOkBtn->resize(182,70);
    stateOkBtn->move(170,494);
    stateOkBtn->setFlat(true);
    stateOkBtn->setFocusPolicy(Qt::NoFocus);
    stateOkBtn->setEnabled(false);
    stateOkBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/info/registerBtn_bg.png)");

    stateCancelBtn = new QPushButton(statePage);
    stateCancelBtn->resize(182,70);
    stateCancelBtn->move(453,494);
    stateCancelBtn->setFlat(true);
    stateCancelBtn->setFocusPolicy(Qt::NoFocus);
    stateCancelBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/exitBtn.png)");

    stack->addWidget(statePage);

    connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(checkBox_slots(int)));
    connect(stateOkBtn,SIGNAL(clicked()),this,SLOT(stateOkBtn_slots()));
    connect(stateCancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtn_slots()));
}


/****************************************************************
**
*	函数名称：	initpage
*	函数功能：	初始化界面布局
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void infoinputpage::initPage()
{

    QWidget *registerPage = new QWidget(this);
    registerPage->setObjectName("QWidget");
    registerPage->setStyleSheet("#QWidget{background-image: url(:/page/images/info/beijing.png)}");

    QFont font;
    font.setFamily(QString::fromUtf8("wenquanyi"));
    font.setPointSize(16);

    QPalette fontcolor;
    fontcolor.setColor(QPalette::ButtonText,Qt::white);

    QLabel *promptLabel = new QLabel(registerPage);
    promptLabel->resize(372,35);
    promptLabel->move(30,10);
    promptLabel->setStyleSheet("border-image: url(:/page/images/info/word1.png)");


    QLabel *phoneLabel = new QLabel(registerPage);
    phoneLabel->resize(223,31);
    phoneLabel->move(105,60);
    phoneLabel->setStyleSheet("border-image: url(:/page/images/info/word2.png)");

    mPhoneLineEdit = new CKeyBoardLineEdit(registerPage);
    mPhoneLineEdit->setMaxLength(11);
    mPhoneLineEdit->resize(300,40);
    mPhoneLineEdit->move(350,60);
    mPhoneLineEdit->installEventFilter(registerPage);

    QLabel *idLabel = new QLabel(registerPage);
    idLabel->resize(223,30);
    idLabel->move(105,120);
    idLabel->setStyleSheet("border-image: url(:/page/images/info/word3.png)");

    mIdLineEdit = new CKeyBoardLineEdit(registerPage);
    mIdLineEdit->setMaxLength(18);
    mIdLineEdit->resize(300,40);
    mIdLineEdit->move(350,120);


    QLabel *mM1CardLabel = new QLabel(registerPage);
    mM1CardLabel->resize(285,30);
    mM1CardLabel->move(40,189);
    mM1CardLabel->setStyleSheet("border-image: url(:/page/images/info/word4.png)");

    mM1CardLineEdit = new CKeyBoardLineEdit(registerPage);
    mM1CardLineEdit->resize(300,40);
    mM1CardLineEdit->move(350,189);
    mM1CardLineEdit->installEventFilter(this);

    QLabel *mCustomerTypeLabel = new QLabel(registerPage);
    mCustomerTypeLabel->resize(223,30);
    mCustomerTypeLabel->move(100,260);
    mCustomerTypeLabel->setStyleSheet("border-image: url(:/page/images/info/word5.png)");


    QStringList customerTypeList;
    customerTypeList << "快递员" << "用户";
    mCustomerTypeComBox = new QComboBox(registerPage);
    mCustomerTypeComBox->resize(300,40);
    mCustomerTypeComBox->move(350,260);
    mCustomerTypeComBox->addItems(customerTypeList);
    mCustomerTypeComBox->setStyleSheet("font-family:wenquanyi; font-size:24px");

    connect(mCustomerTypeComBox,SIGNAL(currentIndexChanged(const QString &)),this,SLOT(hideCompany_slots(const QString &)));



    companyLabel = new QLabel(registerPage);
    companyLabel->resize(223,30);
    companyLabel->move(100,335);
    companyLabel->setStyleSheet("border-image: url(:/page/images/info/word6.png)");

    setStyleSheet("QComboBox { min-height: 40px; min-width: 60px; }"
        "QComboBox QAbstractItemView::item { min-height: 50px; min-width: 60px; }");
    mCompanyComBox = new QComboBox(registerPage);
  //  mCompanyComBox->setStyleSheet("QComboBox{border:1px solid gray;}"
  //    "QComboBox QAbstractItemView::item{height:20px;}" //下拉选项高度
  //    "QComboBox::drop-down{border:2px;}"); //下拉按钮
  //  mCompanyComBox->setView(new QListView());
 //   mCompanyComBox->setFont(font);
    mCompanyComBox->setView(new QListView());
    mCompanyComBox->setMaxVisibleItems(5);
    mCompanyComBox->setStyleSheet("font-family:wenquanyi; font-size:24px");
    mCompanyComBox->resize(300,40);
    mCompanyComBox->move(350,335);


    okBtn = new QPushButton(registerPage);
    okBtn->resize(182,70);
    okBtn->move(170,494);
    okBtn->setFlat(true);
    okBtn->setFocusPolicy(Qt::NoFocus);
    okBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/okBtn.png)");

    cancelBtn = new QPushButton(registerPage);
    cancelBtn->resize(182,70);
    cancelBtn->move(453,494);
    cancelBtn->setFlat(true);
    cancelBtn->setFocusPolicy(Qt::NoFocus);
    cancelBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/exitBtn.png)");

    stack->addWidget(registerPage);

    progress.setMinimum(0);
    progress.setMaximum(0);
    progress.setValue(0);
    progress.setCancelButton(0);
    progress.setWindowFlags(Qt::FramelessWindowHint);
    progress.setLabelText(tr("处理中，请稍后..."));

    connect(okBtn,SIGNAL(clicked()),this,SLOT(okBtn_slots()));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtn_slots()));
    connect(mPhoneLineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));
    connect(mIdLineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));
    connect(mM1CardLineEdit,SIGNAL(btnPress()),this,SIGNAL(resetTime()));
}

/****************************************************************
**
*	函数名称：	judgeInfo
*	函数功能：	校准信息
* 	函数参数：	无
* 	返回值  :    无
*
****************************************************************/
void infoinputpage::checkBox_slots(int id)
{
    emit resetTime();
    if(0 == id)
    {
        stateOkBtn->setEnabled(true);
        stateOkBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/info/registerBtn.png)");
        checkBox->setCheckState(Qt::Checked);
        opposeCheckBox->setCheckState(Qt::Unchecked);

    }
    else if(1 == id)
    {
        stateOkBtn->setEnabled(false);
        stateOkBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/info/registerBtn_bg.png)");
        checkBox->setCheckState(Qt::Unchecked);
        opposeCheckBox->setCheckState(Qt::Checked);

    }
}


void infoinputpage::stateOkBtn_slots()
{
    emit resetTime();
    stack->setCurrentIndex(1);
}

/****************************************************************
**
*	函数名称：	judgeInfo
*	函数功能：	校准信息
* 	函数参数：	无
* 	返回值  :    无
*
****************************************************************/

bool infoinputpage::judgeInfo()
{
    QRegExp rxCellPhone("^1\\d{10}");
    QRegExp pidrex("[0-9]\\d{16}[0-9xX]|[0-9]\\d{14}");
  //  QRegExp rxId("[0-9]");
    bool ret = true;
    QString info;


    if(this->mCompanyComBox->currentText().isEmpty() && mCustomerTypeComBox->currentIndex() == 0)
    {
        info = "请选择快递公司";
        ret = false;
    }
    else if(!rxCellPhone.exactMatch(this->mPhoneLineEdit->text().trimmed()))
    {
        info = "请输入正确的手机号码";
        ret = false;
    }
    else if(!pidrex.exactMatch(this->mIdLineEdit->text().trimmed()))
    {
        info = "请输入正确的身份证号";
        ret = false;
    } 
    else if(this->mM1CardLineEdit->text().trimmed().isEmpty())
    {
        info = "请输入正确的M1卡号";
        ret = false;
    }

    if(!ret)
    {
        myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"), info);
    }

    return ret;

}
void infoinputpage::clearInfo()
{
    this->mPhoneLineEdit->clear();
    this->mM1CardLineEdit->clear();
    this->mIdLineEdit->clear();
    this->mCustomerTypeComBox->setCurrentIndex(0);
    this->mCompanyComBox->setCurrentIndex(0);
}

void infoinputpage::okBtn_slots()
{
    resetTime();
    saveExpressEmpResp respone;
    if(judgeInfo())
    {
        this->okBtn->setEnabled(false);
        this->cancelBtn->setEnabled(false);
        /*姓名，身份证号，手机号码，M1卡号，客户类型,快递公司*/

        QString customerName = "";
        QString cardId = this->mIdLineEdit->text().trimmed();
        QString mobilePhone = this->mPhoneLineEdit->text().trimmed();
        QString m1cardId = mM1CardLineEdit->text().trimmed();
        QString customerType = mCustomerTypeComBox->currentText().trimmed();
        int arryValue = mCompanyNameInfo.indexOf(mCompanyComBox->currentText());
        QString companyId = mCompanyIdInfo.at(arryValue);

        QString myAccount = "";
        QString myPasswd = "";

        progress.show();
        if(customerType == "快递员")
        {
            SCServerComm::instance()->saveExpressEmp(optM1,m1cardId,companyId,customerName,cardId,mobilePhone,myAccount,myPasswd,&respone);
        }
        else
        {
            SCServerComm::instance()->addVipCustomer(optM1,m1cardId,customerName,mobilePhone,cardId,&respone);
        }

        progress.hide();
        if(respone.success() == "true")
        {
            myMessageBox.myPrompt(QMessageBox::Information,tr("提示"),tr("注册成功"));
            clearInfo();
        }
        else if(respone.success() == RET_FALSE && (respone.errCode() == EXIST_MI_ERROR || respone.errCode() == EXIST_CUSTOMER))
        {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("您输入的会员卡号已注册。"));
        }
        else if(respone.success() == RET_FALSE && respone.errCode() == EXIST_IC_ERROR)
        {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("您输入的身份证号已注册。"));
        }
        else if(respone.success() == RET_FALSE && respone.errCode() == EXIST_PHONE_ERROR)
        {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("您输入的手机号码已注册。"));
        }      
        else
        {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("网络异常,请稍后再试。"));
        }
        this->okBtn->setEnabled(true);
        this->cancelBtn->setEnabled(true);
    }
}

void infoinputpage::cancelBtn_slots()
{
    clearInfo();
    optM1.clear();
    mCompanyIdInfo.clear();
    mCompanyNameInfo.clear();
    mCompanyComBox->clear();
    SCMainCtrl::instance()->enterStatus(ST_HOME, "");
}


//void infoinputpage::maintainPage()
//{
//    clearInfo();
//    optM1.clear();
//    mCompanyInfo.clear();
//    mCompanyComBox->clear();
//    SCMainCtrl::instance()->enterStatus(ST_DEVICE_REST, "");
//}

void infoinputpage::readCardInfo(QString data)
{
    if(isM1IdWorking)
    {
        QWidget *fWidget = qApp->focusWidget();
        QLineEdit *myobj = static_cast<QLineEdit*>(fWidget);
        myobj->setText(data);
    }
}

void infoinputpage::readInfo(QByteArray data)
{

    stack->setCurrentIndex(0);
    opposeCheckBox->setCheckState(Qt::Checked);

    stateOkBtn->setEnabled(false);
    stateOkBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/info/registerBtn_bg.png)");
    textContent->verticalScrollBar()->setValue(0);
    isM1IdWorking = false;

    mPhoneLineEdit->setFocus();
    QDataStream in(&data,QIODevice::ReadOnly);
    in >> optM1 >> mCompanyIdInfo >> mCompanyNameInfo;

    this->mCompanyComBox->addItems(mCompanyNameInfo);

}

bool infoinputpage::eventFilter(QObject *obj, QEvent *event)
{

    if(obj == mM1CardLineEdit)
    {
        if(event->type() == QEvent::FocusIn)
        {
            isM1IdWorking = true;
        }
        else if(event->type() == QEvent::FocusOut)
        {
            isM1IdWorking = false;
        }
    }
    return QDialog::eventFilter(obj,event);
}

void infoinputpage::hideCompany_slots(const QString & tempStr)
{
    if(tempStr == "快递员")
    {
        mCompanyComBox->show();
        companyLabel->show();
    }
    else
    {
        mCompanyComBox->hide();
        companyLabel->hide();
    }
}

void infoinputpage::changeEvent(QEvent *e)
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
