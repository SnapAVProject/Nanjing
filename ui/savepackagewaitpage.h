#ifndef SAVEPACKAGEWAITPAGE_H
#define SAVEPACKAGEWAITPAGE_H

#include <QDialog>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include "servercomm.h"
#include "lockdrv.h"
#include "mymessage.h"

class savepackagewaitpage : public QDialog
{
     Q_OBJECT
public:
    savepackagewaitpage();

    void readInfo(QByteArray param);

public slots:
    void okBtnSlot();
    void cancelBtnSlot();
    void overTime();
    void queryLockStatus();
    void openLockSlot();

    void changeBoxSlot();

private:

    void initWaitpage();

    void clearInfo();

    void setBtnEnable(bool);

    QLabel *timeLabel;
    QLabel *doorId;
    QLabel *doorCellId;
    int waitSaveTime;

    QTimer isOverTime;
    QTimer queryLock;

    QPushButton *okBtn;
    QPushButton *cancelBtn;

    bool isOpenBox;                             //判断箱门是否关闭
    QString M1Id;
    QString boxType;                            //选中的箱体类型

    QMap<QString,qint8> notOpenBoxList;         //无法打开的箱门列表
    QMap<QString,qint8> notCheckBoxList;        //物品检测损坏的的箱门列表
    packInfo daemonInfo;                        //发送给后台的数据

    CLockDrv lockoperation;

    CMessageBox *myMessageBox;
    CMessageBox *myBox;
};

#endif // SAVEPACKAGEWAITPAGE_H
