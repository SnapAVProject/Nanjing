#ifndef EXCEPTIONPAGE_H
#define EXCEPTIONPAGE_H

#include <QDialog>
#include "mymessage.h"
#include "cpromptinfo.h"
#include <QProgressDialog>

namespace Ui {
class exceptionpage;
}

class exceptionpage : public QDialog
{
    Q_OBJECT
    
public:
    explicit exceptionpage(QWidget *parent = 0);
    ~exceptionpage();

    void setBtnEnable(bool status);
    qint8 getIsWorking();

signals:
    void resetTime();
public slots:
    void openDoor_slots();
    void takeDelayPackage();
    void courier_slots();
    void exit_slots();
    void noCardLogin();
    void recvCardInfo(QString);    
    
private:
    Ui::exceptionpage *ui;

    void initPage();

    QProgressDialog progress;
    QPushButton *openDoorBtn;
    QPushButton *exitBtn;
    QPushButton *delayPackageBtn;
    QPushButton *courierBtn;

    CMessageBox *myMessageBox;
    CPromptInfo *promptInfo;
    qint8 WorkingStatus; //0、无工作状态  1、空箱处理状态  2、快递员取滞留件状态
};

#endif // EXCEPTIONPAGE_H
