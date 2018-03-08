#ifndef VIPBINDPAGE_H
#define VIPBINDPAGE_H

#include <QDialog>
#include "mylineedit.h"
#include "mymessage.h"

class QLineEdit;
class QPhusButton;

class vipbindpage : public QDialog
{
    Q_OBJECT
public:
    explicit vipbindpage(QWidget *parent = 0);
    void setM1Card(QString);

signals:
    void resetTime();
public slots:

    void start_bind();
    void closePage();

private:
    void initPage();
    bool judgeContent();

    QLineEdit *m1Card_lineEdit;
    CKeyBoardLineEdit *phone_lineEdit;
    CKeyBoardLineEdit *code_lineEdit;

    QPushButton *okBtn;
    QPushButton *quitBtn;
    CMessageBox myMessageBox;
    
};

#endif // VIPBINDPAGE_H
