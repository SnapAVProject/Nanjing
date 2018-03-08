#ifndef EXCEPTIONSTATUS_H
#define EXCEPTIONSTATUS_H

#include "status.h"
#include "exceptionpage.h"

class CExceptionStatus : public CStatus
{
    Q_OBJECT
public:
    CExceptionStatus(STATUS status = ST_EXCEPTION_HANDLE);

    void enter(QByteArray param);
    void leave(QByteArray param);

    void cardInputProc(QByteArray param);

signals:

public slots:
    void resetStart();
    void jumpPage();
private:
    exceptionpage *abnormalpage;
    CMessageBox *myMessageBox;

};

#endif // EXCEPTIONSTATUS_H
