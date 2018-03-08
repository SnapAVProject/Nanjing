#ifndef PRICECHECK_H
#define PRICECHECK_H

#include <QDialog>
#include "servercomm.h"
#include <QLabel>

namespace Ui {
class pricecheck;
}

class pricecheck : public QDialog
{
    Q_OBJECT
    
public:
    explicit pricecheck(QWidget *parent = 0);
    ~pricecheck();
    void initPage();
    void readInfo();
public slots:
    void signOutPushButton_clicked();

private:
    Ui::pricecheck *ui;
    QLabel *bigPrice;
    QLabel *midPrice;
    QLabel *smallPrice;

    QLabel *VIPEveryPrice;
    QLabel *VIPPrice;
    QLabel *APPVIPPrice;
    QLabel *delayTime;
    QLabel *delayCoin;
    QLabel *outTime;
    QLabel *outCoin;
};

#endif // PRICECHECK_H
