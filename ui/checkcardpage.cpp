#include "checkcardpage.h"
#include "mainctrl.h"
#include <QLabel>

checkcardpage::checkcardpage(QWidget *parent) :
    QDialog(parent)
{
    initPage();
}



void checkcardpage::initPage()
{
    this->setObjectName("QWidget");
    this->setStyleSheet("#QWidget{background-image: url(:/public/images/public/beijing.png)}");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800,600);

    QLabel *prompt = new QLabel(this);
    prompt->resize(464,41);
    prompt->move(10,30);
    prompt->setStyleSheet("border-image: url(:/page/images/querybalance/word1.png)");

    QLabel *label = new QLabel(this);
    label->resize(231,39);
    label->move(30,206);
    label->setStyleSheet("border-image: url(:/page/images/checkcard/word.png)");

    lineEdit = new QLineEdit(this);
    lineEdit->resize(350,40);
    lineEdit->move(330,206);
    lineEdit->setStyleSheet("QLineEdit{font-family:wenquanyi; font-size:30px;padding: 1px;border-style: solid;border: 2px solid gray;border-radius: 8px;}");

    exitBtn = new QPushButton(this);
    exitBtn->setFixedSize(182,70);
    exitBtn->move(320,480);
    exitBtn->setFlat(true);
    exitBtn->setFocusPolicy(Qt::NoFocus);
    exitBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/exitBtn.png)");


    connect(exitBtn,SIGNAL(clicked()),this,SLOT(exitBtn_slots()));
}

void checkcardpage::readCardInfo(QString data)
{
    lineEdit->setText(data);
}




void checkcardpage::exitBtn_slots()
{
    lineEdit->clear();
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
