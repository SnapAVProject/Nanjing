#ifndef SAVEPACKAGEWAITSTATUS_H
#define SAVEPACKAGEWAITSTATUS_H

#include "status.h"
#include "savepackagewaitpage.h"
#include <QTimer>

class CSavepackagewaitStatus : public CStatus
{
	Q_OBJECT
public:
    CSavepackagewaitStatus(STATUS status = ST_SAVE_DELIVERY_WAIT);
    void enter(QByteArray param);
    void leave(QByteArray param);

public slots:
    void leaveSlots();

private:
	QTimer mTimer;
    savepackagewaitpage *savewaitpage;
};

#endif // SAVEPACKAGEWAITSTATUS_H
