#ifndef EXCEPTIONOPENDOORSTATUS_H
#define EXCEPTIONOPENDOORSTATUS_H

#include "status.h"
#include "exceptionopendoorpage.h"

class CExceptionOpenDoorStatus : public CStatus
{
    Q_OBJECT
public:
    CExceptionOpenDoorStatus(STATUS status = ST_EXCEPTION_OPENDOOR);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void keyInputProc(QByteArray param);

public slots:
    void resetStart();
    void jumpPage();

private:
    exceptionopendoorpage *opendoorpage;
};

#endif // EXCEPTIONOPENDOORSTATUS_H
