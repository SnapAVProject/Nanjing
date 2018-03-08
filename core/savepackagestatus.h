#ifndef SAVEPACKAGESTATUS_H
#define SAVEPACKAGESTATUS_H

#include "status.h"
#include "savepackagepage.h"
#include "mymessage.h"
#include <QTimer>


class CSavepackageStatus : public CStatus
{
    Q_OBJECT

public:
    CSavepackageStatus(STATUS status = ST_SAVE_DELIVERY_MENU);
    void enter(QByteArray param);
    void leave(QByteArray param);

    void keyInputProc(QByteArray param);

public slots:
    void timeOutComing();
    void resetStart();
    void jumpPage();

    void leaveSlots();


private:
    savepackagepage *savepackage;
    QByteArray data;
    QTimer mTimer;
    CMessageBox myMessageBox;
};

#endif // SAVEPACKAGESTATUS_H
