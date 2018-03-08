#ifndef USEGUIDESTATUS_H
#define USEGUIDESTATUS_H

#include "status.h"
#include "useguidepage.h"

class CUseguideStatus:public CStatus
{
    Q_OBJECT
public:
    CUseguideStatus(STATUS status = ST_USE_GUIDE);

    void enter(QByteArray param);
    void leave(QByteArray param);

public slots:
    void resetStart();
    void jumpPage();

private:
    useguidepage *useguide;
};

#endif // USEGUIDESTATUS_H
