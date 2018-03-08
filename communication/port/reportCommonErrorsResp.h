#ifndef REPORTCOMMONERRORSRESP_H
#define REPORTCOMMONERRORSRESP_H

#include <QObject>
#include <QString>
class reportCommonErrorsResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success     WRITE setSuccess)

public:

    reportCommonErrorsResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}

    ~reportCommonErrorsResp()
    {
    }

    void setSuccess(QString success)
    {
        mSuccess = success;
    }

private:
    QString mSuccess ;  //终端异常上报
};

#endif // REPORTCOMMONERRORSRESP_H
