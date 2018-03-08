#include "mylineedit.h"
#include "driver.h"
#include <QDebug>

#define SHOW_TIME 60*1000

CKeyBoardLineEdit::CKeyBoardLineEdit(QWidget * parent):
    QLineEdit(parent)
{

    timer.setInterval(SHOW_TIME);
    timer.setSingleShot(true);

    this->setStyleSheet("QLineEdit{font-family:wenquanyi; font-size:30px;padding: 1px;border-style: solid;border: 2px solid gray;border-radius: 8px;}");
    connect(&mykeyboard,SIGNAL(btn0signal()),this,SLOT(btn0slot()));
    connect(&mykeyboard,SIGNAL(btn1signal()),this,SLOT(btn1slot()));
    connect(&mykeyboard,SIGNAL(btn2signal()),this,SLOT(btn2slot()));
    connect(&mykeyboard,SIGNAL(btn3signal()),this,SLOT(btn3slot()));
    connect(&mykeyboard,SIGNAL(btn4signal()),this,SLOT(btn4slot()));
    connect(&mykeyboard,SIGNAL(btn5signal()),this,SLOT(btn5slot()));
    connect(&mykeyboard,SIGNAL(btn6signal()),this,SLOT(btn6slot()));
    connect(&mykeyboard,SIGNAL(btn7signal()),this,SLOT(btn7slot()));
    connect(&mykeyboard,SIGNAL(btn8signal()),this,SLOT(btn8slot()));
    connect(&mykeyboard,SIGNAL(btn9signal()),this,SLOT(btn9slot()));
    connect(&mykeyboard,SIGNAL(btnXsignal()),this,SLOT(btnXslot()));
    connect(&mykeyboard,SIGNAL(btnspacesignal()),this,SLOT(btnspaceslot()));
    connect(&mykeyboard,SIGNAL(btnwellsignal()),this,SLOT(btnwellslot()));
    connect(&mykeyboard,SIGNAL(backbtnsignal()),this,SLOT(backbtnslot()));
    connect(&mykeyboard,SIGNAL(okbtnsignal()),this,SLOT(okbtnslot()));
    connect(&mykeyboard,SIGNAL(cancelbtnsignal()),this,SLOT(cancelbtnslot()));

    connect(&mykeyboard,SIGNAL(btn0signal()),this,SIGNAL(btnPress()));
    connect(&mykeyboard,SIGNAL(btn1signal()),this,SIGNAL(btnPress()));
    connect(&mykeyboard,SIGNAL(btn2signal()),this,SIGNAL(btnPress()));
    connect(&mykeyboard,SIGNAL(btn3signal()),this,SIGNAL(btnPress()));
    connect(&mykeyboard,SIGNAL(btn4signal()),this,SIGNAL(btnPress()));
    connect(&mykeyboard,SIGNAL(btn5signal()),this,SIGNAL(btnPress()));
    connect(&mykeyboard,SIGNAL(btn6signal()),this,SIGNAL(btnPress()));
    connect(&mykeyboard,SIGNAL(btn7signal()),this,SIGNAL(btnPress()));
    connect(&mykeyboard,SIGNAL(btn8signal()),this,SIGNAL(btnPress()));
    connect(&mykeyboard,SIGNAL(btn9signal()),this,SIGNAL(btnPress()));
    connect(&mykeyboard,SIGNAL(btnXsignal()),this,SIGNAL(btnPress()));
    connect(&mykeyboard,SIGNAL(btnspacesignal()),this,SIGNAL(btnPress()));
    connect(&mykeyboard,SIGNAL(btnwellsignal()),this,SIGNAL(btnPress()));
    connect(&mykeyboard,SIGNAL(backbtnsignal()),this,SIGNAL(btnPress()));
    connect(&mykeyboard,SIGNAL(okbtnsignal()),this,SIGNAL(btnPress()));
    connect(&mykeyboard,SIGNAL(cancelbtnsignal()),this,SIGNAL(btnPress()));

    connect(&timer,SIGNAL(timeout()),this,SLOT(timeOver()));

}

CKeyBoardLineEdit::~CKeyBoardLineEdit()
{

}

/***************************************************************

        函数名称：focusInEvent()
        函数功能：当编辑框获取焦点的响应
        函数参数： 无
        返回值：无

***************************************************************/
void CKeyBoardLineEdit::mouseReleaseEvent(QMouseEvent *)
{
   //  QWidget::clearFocus();  //获取软盘同时清除焦点
   // mykeyboard.setLineEditData(this->text());

    if(!this->isReadOnly())
    {
        emit btnPress();
        timer.start();
        mykeyboard.show();
    }


}


void CKeyBoardLineEdit::focusInEvent(QFocusEvent *event)
{
    QLineEdit::focusInEvent(event);
}




void CKeyBoardLineEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    timer.stop();
    mykeyboard.close();

}



void CKeyBoardLineEdit::btn0slot()
{
   timer.setInterval(SHOW_TIME);

   this->insert("0");
}


void CKeyBoardLineEdit::btn1slot()
{
    timer.setInterval(SHOW_TIME);

    this->insert("1");
}

void CKeyBoardLineEdit::btn2slot()
{
    timer.setInterval(SHOW_TIME);

    this->insert("2");
}

void CKeyBoardLineEdit::btn3slot()
{
    timer.setInterval(SHOW_TIME);

    this->insert("3");
}

void CKeyBoardLineEdit::btn4slot()
{
    timer.setInterval(SHOW_TIME);

    this->insert("4");
}

void CKeyBoardLineEdit::btn5slot()
{
    timer.setInterval(SHOW_TIME);

    this->insert("5");
}

void CKeyBoardLineEdit::btn6slot()
{
    timer.setInterval(SHOW_TIME);

    this->insert("6");
}


void CKeyBoardLineEdit::btn7slot()
{
    timer.setInterval(SHOW_TIME);
    this->insert("7");
}

void CKeyBoardLineEdit::btn8slot()
{
    timer.setInterval(SHOW_TIME);
    this->insert("8");
}

void CKeyBoardLineEdit::btn9slot()
{
    timer.setInterval(SHOW_TIME);
    this->insert("9");
}

void CKeyBoardLineEdit::btnXslot()
{
    timer.setInterval(SHOW_TIME);
    this->insert("X");
}

void CKeyBoardLineEdit::btnspaceslot()
{
    timer.setInterval(SHOW_TIME);
    this->insert(".");
}

void CKeyBoardLineEdit::btnwellslot()
{
    timer.setInterval(SHOW_TIME);
    this->insert("#");
}

void CKeyBoardLineEdit::backbtnslot()
{
   timer.setInterval(SHOW_TIME);

   this->backspace();
}

void CKeyBoardLineEdit::okbtnslot()
{
   timer.stop();
   this->focusNextChild();
   emit okBtnPress();
   mykeyboard.close();
}

void CKeyBoardLineEdit::cancelbtnslot()
{
   timer.stop();
   this->clear();
   mykeyboard.close();
}

void CKeyBoardLineEdit::keyPressEvent(QKeyEvent *event)
{

    if(!this->isReadOnly())
    switch(event->key())
    {
    case Qt::Key_0:
        btn0slot();
        break;
    case Qt::Key_1:
        btn1slot();
        break;
    case Qt::Key_2:
        btn2slot();
        break;
    case Qt::Key_3:
        btn3slot();
        break;
    case Qt::Key_4:
        btn4slot();
        break;
    case Qt::Key_5:
        btn5slot();
        break;
    case Qt::Key_6:
        btn6slot();
        break;
    case Qt::Key_7:
        btn7slot();
        break;
    case Qt::Key_8:
        btn8slot();
        break;
    case Qt::Key_9:
        btn9slot();
        break;
    case 0x20:                   //空格
        btnspaceslot();
        break;
    case 0x58:                   //*
        btnXslot();
        break;
    case 0x23:                    //#
        btnwellslot();
        break;
    case 0x08:                    //退格
        backbtnslot();
        break;
    case 0x18:                    //取消
        cancelbtnslot();
        break;
    case 0x0D:                     //确定
        okbtnslot();
        break;

    }

}

void CKeyBoardLineEdit::timeOver()
{
    mykeyboard.close();
}
