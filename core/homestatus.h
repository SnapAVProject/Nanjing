#ifndef HOMESTATUS_H
#define HOMESTATUS_H


#include "status.h"
#include "homepage.h"
#include "mainctrl.h"
#include "m1CardLoginResp.h"
#include "mymessage.h"
#include <QProgressDialog>



class CHomeStatus : public CStatus
{
     Q_OBJECT
public:
    CHomeStatus(STATUS status = ST_HOME);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void coinInputProc(QByteArray param);
    void cardInputProc(QByteArray param);

    QByteArray waitconfirmByteArray(m1CardLoginResp &respone,QString personType,bool isFix);   //等待确认的包裹信息
    QByteArray savecellByteArray(QByteArray param);               //存件的箱体可用量信息

    bool specialcard(QByteArray param);                           //判断是否是特别的卡号

signals:
    void cardcoming(QString);

public slots:
    void updateCell();
    void takeDelayPackage();

private:
    HomePage*   mPage;
    CMessageBox *myMessageBox;
    CMessageBox myBox;

    QString mM1Card;
    QString mMobilePhone;
    QProgressDialog progress;
};

#endif // HOMESTATUS_H
