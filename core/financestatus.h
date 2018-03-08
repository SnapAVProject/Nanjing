#ifndef FINANCESTATUS_H
#define FINANCESTATUS_H

#include "status.h"
#include "financepage.h"

class CFinanceStatus : public CStatus
{
    Q_OBJECT
public:
    CFinanceStatus(STATUS status = ST_FINANCE);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void keyInputProc(QByteArray param);

public slots:
    void resetStart();
    void jumpPage();

private:
    financepage *finance;

};

#endif // FINANCESTATUS_H
