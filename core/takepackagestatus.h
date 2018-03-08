#ifndef CTAKEPACKAGESTATUS_H
#define CTAKEPACKAGESTATUS_H

#include "status.h"
#include "takepackagepage.h"

class CTakePageStatus : public CStatus
{
    Q_OBJECT
public:
    CTakePageStatus(STATUS status = ST_GET_DELIVERY);

    void enter(QByteArray param);
    void leave(QByteArray param);


   void coinInputProc(QByteArray param);
   void keyInputProc(QByteArray param);
   void cardInputProc(QByteArray param);


signals:
 //  void coincoming();
 //  void cardcoming(QString);

public slots:
   void resetStart();
   void jumpPage();
   void leaveSlots();
    /*

    void  onSaveButtonProc(){
        SCMainCtrl::instance()->enterStatus(ST_NO_CARD_SAVE, "");
    }
*/
private:
    takepackagepage *getpackage;
};

#endif // CTAKEPACKAGESTATUS_H
