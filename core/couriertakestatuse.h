#ifndef COURIERTAKEPACKAGESTATUS_H
#define COURIERTAKEPACKAGESTATUS_H

#include "status.h"
#include "courierpickage.h"

class CourierTakePackageStatus : public CStatus
{
    Q_OBJECT

public:
    explicit CourierTakePackageStatus(STATUS status = ST_COURIER_TAKE);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void keyInputProc(QByteArray param);

signals:

public slots:
    void resetStart();
    void jumpPage();

private:
    courierpickpage *courierpick;
    
};

#endif // COURIERTAKEPACKAGESTATUS_H
