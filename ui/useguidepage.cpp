#include "useguidepage.h"
#include "mainctrl.h"
#include <QStackedLayout>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>

useguidepage::useguidepage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    initPage();
}



void useguidepage::initPage()
{
    this->setObjectName("QWidget");
    this->setStyleSheet("#QWidget{border-image: url(:/page/images/useguide/01.png)}");


    QPushButton *prevBtn = new QPushButton(this);
    prevBtn->setFixedSize(182,70);
    prevBtn->move(70,530);
    prevBtn->setFlat(true);
    prevBtn->setFocusPolicy(Qt::NoFocus);
    prevBtn->setStyleSheet("background-color:transparent;background-image: url(:/public/images/public/frontBtn.png)");

    QPushButton *nextBtn = new QPushButton(this);
    nextBtn->setFixedSize(182,70);
    nextBtn->move(290,530);
    nextBtn->setFlat(true);
    nextBtn->setFocusPolicy(Qt::NoFocus);
    nextBtn->setStyleSheet("background-color:transparent;background-image: url(:/public/images/public/backBtn.png)");

    QPushButton *exitBtn = new QPushButton(this);
    exitBtn->setFixedSize(182,70);
    exitBtn->move(510,530);
    exitBtn->setFlat(true);
    exitBtn->setFocusPolicy(Qt::NoFocus);
    exitBtn->setStyleSheet("background-color:transparent;background-image:url(:/public/images/public/exitBtn.png)");


    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(prevBtn,0);
    buttonGroup->addButton(nextBtn,1);
    buttonGroup->addButton(exitBtn,2);
    buttonGroup->setExclusive(true);  //互斥


    connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(cutUseGuidePage(int)));
    connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SIGNAL(resetTime()));

    mCurPage = 0;
    this->resize(800,600);


}

void useguidepage::readInfo()
{
    mCurPage = 0;
    this->setStyleSheet("#QWidget{border-image: url(:/page/images/useguide/01.png)}");
}

void useguidepage::showCutPage()
{
    if(0 == mCurPage)
    {
        this->setStyleSheet("#QWidget{border-image: url(:/page/images/useguide/01.png)}");
    }
    else if (1 == mCurPage)
    {
        this->setStyleSheet("#QWidget{border-image: url(:/page/images/useguide/02.png)}");
    }
    else if (2 == mCurPage)
    {
        this->setStyleSheet("#QWidget{border-image: url(:/page/images/useguide/03.png)}");
    }
    else if (3 == mCurPage)
    {
        this->setStyleSheet("#QWidget{border-image: url(:/page/images/useguide/04.png)}");
    }
}


void useguidepage::cutUseGuidePage(int buttonid)
{
    if(buttonid == 0)
    {
        if(mCurPage == 0)
        {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("已至首页。"));

        }
        else
        {
            mCurPage--;
            showCutPage();
        }
    }
    else if(buttonid == 1)
    {
        if(mCurPage == 3)
        {
            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示"),tr("已至尾页。"));
        }
        else
        {
            mCurPage++;
            showCutPage();
        }
    }
    else if(buttonid == 2)
    {
        SCMainCtrl::instance()->enterStatus(ST_HOME,"");
    }

}
