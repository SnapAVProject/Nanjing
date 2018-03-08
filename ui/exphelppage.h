#ifndef EXPHELPPAGE_H
#define EXPHELPPAGE_H

#include <QDialog>
#include <QTableView>
#include <QLabel>
#include <QPushButton>
#include "takeviewmodel.h"
#include "btndelegate.h"
#include "lockdrv.h"

namespace Ui {
class exphelppage;
}

class exphelppage : public QDialog
{
    Q_OBJECT
    
public:
    explicit exphelppage(QWidget *parent = 0);

    void readInfo(QByteArray);
    ~exphelppage();
signals:
    void resetTime();
public slots:
    void exit_slots();
    void openCell_slots(int,int);

private:
    Ui::exphelppage *ui;

    void initPage();
    bool againCheckCellProc(QString openCell,QString deliveryId);

    CLockDrv lockoperation ;

    QTableView *view;
    takeViewModel *model;
    BtnDelegate *btnDelegate;
    QList <QStringList> modelData;

    QLabel *packCount;
    QPushButton *exitBtn;

    QString terminal;

    QStringList takeDeliveryList;
    QMap<QString,QString> notOpenCell;
};

#endif // EXPHELPPAGE_H
