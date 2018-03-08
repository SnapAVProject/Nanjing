#ifndef REPAIRCOMMONERRORSRESP_H
#define REPAIRCOMMONERRORSRESP_H


#include <QObject>
#include <QString>

class repairCommonErrorsResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success     WRITE setSuccess)

public:

    repairCommonErrorsResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}

    ~repairCommonErrorsResp()
    {
    }

    void setSuccess(QString success)
    {
        mSuccess = success;
    }

private:
    QString mSuccess ;  //终端异常修复上报
};

#endif // REPAIRCOMMONERRORSRESP_H
