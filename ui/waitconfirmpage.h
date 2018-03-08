#ifndef WAITCONFIRMPAGE_H
#define WAITCONFIRMPAGE_H

#include <QDialog>
#include <QLineEdit>
#include <QStringList>
#include <QByteArray>
#include "mymessage.h"

namespace Ui {
    class waitconfirmpage;
}

class waitconfirmpage : public QDialog {
    Q_OBJECT
public:
    waitconfirmpage(QWidget *parent = 0);
    ~waitconfirmpage();

    void readwaitconfirminfo(QByteArray);

signals:
    void resetTime();

public slots:
    void okBtn_slot();
    void cancelBtn_slot();
    void startCancelPackage_slot();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::waitconfirmpage *ui;

    QString deliveryId;
    QString m1cardId;
    bool isFix;

    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QLineEdit *idLineEdit;
    QLineEdit *phoneLineEdit;
    QLineEdit *boxLineEdit;

    QStringList bigCellIdList;
    QStringList middleCellIdList;
    QStringList smallCellIdList;
    QStringList vegetablesCellidList;
    QStringList blackNameList;
    QStringList orderBigCells;
    QStringList orderMiddleCells;
    QStringList orderSmallCells;
    QStringList orderVegeCells;
    QStringList orderCells;

    QString cellId;
    QString mTariffType;
    QString mBalance;

    CMessageBox myMessageBox;
    CMessageBox recvBox;

    QString personType; //快递员类型

    void initPage();
    int cancelNoDebtUpdateBox(QString);
    void cancelDebtUpdateBox(QString);


};

#endif // WAITCONFIRMPAGE_H
