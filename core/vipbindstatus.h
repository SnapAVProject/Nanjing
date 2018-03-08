#ifndef VIPBINDSTATUS_H
#define VIPBINDSTATUS_H

#include "status.h"
#include "vipbindpage.h"

class CVipBindStatus : public CStatus
{
    Q_OBJECT
public:
    explicit CVipBindStatus(STATUS status = ST_BIND_VIP);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void cardInputProc(QByteArray param);
    void keyInputProc(QByteArray param);
    
signals:
    
public slots:
    void resetStart();
    void jumpPage();

private:
    vipbindpage *bindpage;
    
};

#endif // VIPBINDSTATUS_H
