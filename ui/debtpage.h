#ifndef DEBTPAGE_H
#define DEBTPAGE_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <QTimer>
#include <QPushButton>
#include "mymessage.h"

namespace Ui {
    class debtpage;
}

class QMessageBox;

class CDebtPage : public QDialog {
    Q_OBJECT
public:
    CDebtPage(QWidget *parent = 0);
    ~CDebtPage();

    void setCoinLab(QString);
    void updateCoinLab(QString);
    void debtTimeStart(QString,QString,double,double,double);
    void handleDebtInfo();

    void setBtnEnable(bool);

    void closeCurDebtPage();

    void updateDebtInfo(QString);

    void promptInfo(QMessageBox::Icon icon,const QString & title, const QString & text,Qt::WindowModality model);

signals:
//    void debtPageClose();
public slots:
    void quitDebtPage();
    void timeOver();

    void retBtnStatus();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::debtpage *ui;

    QPushButton *quitBtn;
    QLabel *coinLab;
    QLabel *debtCostLab;     //滞留费
    QLabel *delOwnerCostLab; // 个人需付
    QLabel *ownerHasPayLab;  //已付
    QLabel *vipPercentLab;   //折率

    QTimer timer;

    QString deliveryId;

    qint8 coinNumber;

    CMessageBox *myMessageBox;

    void initPage();
};

#endif // DEBTPAGE_H
