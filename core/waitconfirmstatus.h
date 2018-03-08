#ifndef WAITCONFIRMSTATUS_H
#define WAITCONFIRMSTATUS_H

#include "status.h"
#include "waitconfirmpage.h"

class CWaitconfirmStatus : public CStatus
{
    Q_OBJECT
public:
    CWaitconfirmStatus(STATUS status = ST_PAKEAGE_CONFIRM);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void keyInputProc(QByteArray param);

public slots:
    void jumpPage();

private:
    waitconfirmpage *waitconfirm;
};

#endif // WAITCONFIRMSTATUS_H
