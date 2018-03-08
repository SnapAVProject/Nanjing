#include "cintroductionlxyz.h"
#include "mainctrl.h"
#include <QStackedLayout>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
CIntroductionLXYZ::CIntroductionLXYZ(QDialog *parent) :
    QDialog(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    initPage();
}
void CIntroductionLXYZ::initPage()
{
    this->setObjectName("QWidget");
    this->setStyleSheet("#QWidget{border-image: url(:/page/images/useguide/introduce.png)}");
    this->resize(800,600);



    QPushButton *exitBtn = new QPushButton(this);
    exitBtn->setFixedSize(182,70);
    exitBtn->move(320,510);
    exitBtn->setFlat(true);
    exitBtn->setFocusPolicy(Qt::NoFocus);
    exitBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/exitBtn.png)");

    connect(exitBtn,SIGNAL(clicked()),this,SLOT(exitPage()));

}


void CIntroductionLXYZ::exitPage()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
