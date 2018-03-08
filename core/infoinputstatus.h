#ifndef INFOINPUTSTATUS_H
#define INFOINPUTSTATUS_H

#include "status.h"
#include "infoinputpage.h"

class CInfoinputStatus : public CStatus
{
     Q_OBJECT
public:
    CInfoinputStatus(STATUS status = ST_INFO_INPUT);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void cardInputProc(QByteArray param);
    void keyInputProc(QByteArray param);

signals:
    void cardcoming(QString);

public slots:
    void resetStart();
    void jumpPage();

private:
    infoinputpage *infoinput;
};

#endif // INFOINPUTSTATUS_H
