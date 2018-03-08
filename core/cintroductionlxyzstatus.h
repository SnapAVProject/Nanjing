#ifndef CINTRODUCTIONLXYZSTATUS_H
#define CINTRODUCTIONLXYZSTATUS_H

#include "status.h"
#include "cintroductionlxyz.h"

class CIntroductionLXYZStatus:public CStatus
{
    Q_OBJECT
public:
    CIntroductionLXYZStatus(STATUS status = ST_INTRO_LXYZ);

    void enter(QByteArray param);
    void leave(QByteArray param);

public slots:
    void jumpPage();

private:
    CIntroductionLXYZ *introductionLxyz;
};
#endif // CINTRODUCTIONLXYZSTATUS_H
