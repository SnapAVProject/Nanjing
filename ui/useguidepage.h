#ifndef USEGUIDEPAGE_H
#define USEGUIDEPAGE_H

#include <QDialog>
#include "mymessage.h"

class QStackedLayout;

class useguidepage : public QDialog
{
    Q_OBJECT
public:
    useguidepage();

    void readInfo();

signals:
    void resetTime();

public slots:
    void cutUseGuidePage(int);

private:

    void initPage();
    void showCutPage();

    CMessageBox myMessageBox;

    qint8 mCurPage;    //标识当前页面值
};

#endif // USEGUIDEPAGE_H
