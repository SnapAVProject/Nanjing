#ifndef CARDCHECKSTATUS_H
#define CARDCHECKSTATUS_H


#include "status.h"
#include "checkcardpage.h"
class CCardcheckStatus : public CStatus
{
    Q_OBJECT
public:
    CCardcheckStatus(STATUS status = ST_CHECK_CARD);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void cardInputProc(QByteArray param);

private:
    checkcardpage *cardpage;

public slots:
    void jumpPage();
};

#endif // CARDCHECKSTATUS_H
