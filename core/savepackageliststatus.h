#ifndef SAVEPACKAGELISTSTATUS_H
#define SAVEPACKAGELISTSTATUS_H

#include "status.h"
#include "savepackagelistpage.h"
#include <QTimer>

/*
   ADD_DATA:                              正常操作，显示列表
   ADD_DATA_NO_Wait：                     120秒后，退出操作，待确认
   NO_ADD_DATA：                          120秒后，退出操作，异常箱门
   NO_ADD_DATA_NO_WAITCLOSE_BOX：         不需要添加数据，也没有未关闭的箱门
   */

class CSavepackagelistStatus : public CStatus
{
    Q_OBJECT

public:
    CSavepackagelistStatus(STATUS status = ST_SAVE_DELIVERY_LIST);
    void enter(QByteArray param);
    void leave(QByteArray param);

public slots:
    void timeOutComing();

    void timeStop();

    void leaveSlots();

private:
    savepackagelistpage *savelistpage;
    QTimer mTimer;


};

#endif // SAVEPACKAGELISTSTATUS_H
