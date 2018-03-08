#ifndef PRICECHECKSTATUS_H
#define PRICECHECKSTATUS_H

#include "status.h"
#include "servercomm.h"

class pricecheck;
class QTime;
class CMessageBox;
class pricecheckstatus:public CStatus
{
    Q_OBJECT
public:
    pricecheckstatus(STATUS status = ST_PRICT_CHECK);
    void enter(QByteArray param);
    void leave(QByteArray param);

public slots:
    void jumpPage();

private:
    pricecheck   *pricecheckUi;
    int       fristDay;
    CMessageBox *myMessageBox;
};

#endif // PRICECHECKSTATUS_H
