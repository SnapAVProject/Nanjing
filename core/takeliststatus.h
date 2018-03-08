#ifndef TAKELISTSTATUS_H
#define TAKELISTSTATUS_H


#include "status.h"
#include "cardpickpage.h"

class CTakeListStatus : public CStatus
{
    Q_OBJECT
public:
    CTakeListStatus(STATUS status = ST_GET_CARD);
    void enter(QByteArray param);
    void leave(QByteArray param);

    void keyInputProc(QByteArray param);

public slots:
    void resetStart();
    void jumpPage();

private:
    cardpickpage *pickpage;

};

#endif // TAKELISTSTATUS_H
