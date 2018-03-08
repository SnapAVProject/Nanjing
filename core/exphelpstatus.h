#ifndef EXPHELPSTATUS_H
#define EXPHELPSTATUS_H

#include "status.h"
#include "exphelppage.h"

class CExpHelpStatus : public CStatus
{
    Q_OBJECT
public:
    CExpHelpStatus(STATUS status = ST_COURIER_HELP_TAKE);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void keyInputProc(QByteArray param);

signals:

public slots:
    void resetStart();
    void jumpPage();
private:

    exphelppage *helppage;
};

#endif // EXPHELPSTATUS_H
