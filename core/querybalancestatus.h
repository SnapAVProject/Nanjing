#ifndef QUERYBALANCESTATUS_H
#define QUERYBALANCESTATUS_H

#include "status.h"
#include "querybalancepage.h"

class CQueryBalanceStatus : public CStatus
{
     Q_OBJECT
public:
    CQueryBalanceStatus(STATUS status = ST_QUERY_BALANCE);

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
    querybalancepage *balancepage;

};

#endif // QUERYBALANCESTATUS_H
