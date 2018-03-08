#ifndef DEVICERESTSTATUS_H
#define DEVICERESTSTATUS_H

#include "status.h"
#include "devicerestpage.h"

class CDeviceRestStatus : public CStatus
{
    Q_OBJECT
public:
    CDeviceRestStatus(STATUS status = ST_DEVICE_REST);

    void enter(QByteArray param);
    void leave(QByteArray param);


private:
    devicerestpage *devicerest;
};

#endif // DEVICERESTSTATUS_H
