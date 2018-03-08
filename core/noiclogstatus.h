#ifndef NOICLOGSTATUS_H
#define NOICLOGSTATUS_H

#include "status.h"
#include "noiclogpage.h"

class CNoiclogStatus : public CStatus
{
    Q_OBJECT
public:
    CNoiclogStatus(STATUS status = ST_NO_CARD_SAVE);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void keyInputProc(QByteArray param);


public slots:
    void resetStart();
    void jumpPage();


private:
    noiclogpage *noicsave;

};

#endif // NOICLOGSTATUS_H
