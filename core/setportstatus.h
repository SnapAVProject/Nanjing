#ifndef SETPORTSTATUS_H
#define SETPORTSTATUS_H

#include "status.h"
#include "setportpage.h"

class CSetportstatus : public CStatus
{
    Q_OBJECT
public:
    CSetportstatus(STATUS status = ST_SET_PORT);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void keyInputProc(QByteArray param);
    void coinInputProc(QByteArray param);
    void upsInputProc(QByteArray param);

public slots:
    void resetStart();
    void jumpPage();

private:
    setportpage *portpage;
};

#endif // SETPORTSTATUS_H
