#ifndef COURIERPICKAGE_H
#define COURIERPICKAGE_H

#include <QDialog>
#include <QTableView>
#include "lockdrv.h"
#include "takeviewmodel.h"
#include "mymessage.h"
#include "cardpickpage.h"

class courierpickpage : public QDialog
{
    Q_OBJECT
public:
    explicit courierpickpage(QWidget *parent = 0);

    void readInfo(QByteArray data);

signals:
    void resetTime();
public slots:

    void okBtn_slots();
    void openCell_slots();
    void quit_slots();

private:
    void initPage();
    bool againCheckCellProc(QString,QString);
    void reportInfo();

    CLockDrv lockoperation ;

    QPushButton *openBtn;
    QPushButton *quitBtn;
    QLabel *numberLabel;

    takeViewModel *model;
    QTableView *view;

    QString terminal;
    QList <QStringList> modelData;

    QStringList imageList;

    CMessageBox myMessageBox;
    cardtakeinfo courierTakeInfoPage;
    
};

#endif // COURIERPICKAGE_H
