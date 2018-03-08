#ifndef RECHARGESTATUS_H
#define RECHARGESTATUS_H


#include "status.h"
#include "rechargepage.h"

class CRechargeStatus : public CStatus
{
    Q_OBJECT
public:
    CRechargeStatus(STATUS status = ST_RECHARGE);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void keyInputProc(QByteArray param);

    void cardInputProc(QByteArray param);

signals:
    void cardcoming(QString);

public slots:
    void resetStart();
    void jumpPage();

private:
    rechargepage *rechar;
};

#endif // RECHARGESTATUS_H
