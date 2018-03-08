#ifndef UPDATEACCOUNTPAIDRESP_H
#define UPDATEACCOUNTPAIDRESP_H

#include <QObject>
#include <QString>

class updateAccountPaidResp:public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success     WRITE setSuccess)
    Q_PROPERTY(QString errCode           READ errCode     WRITE setErrcode)
    Q_PROPERTY(QString errMessage        READ errMessage  WRITE setErrMessage)

public:

    updateAccountPaidResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString errMessage() const{return merrMessage;}

    ~updateAccountPaidResp()
    {
    }

    void setSuccess(QString success)
    {
        mSuccess = success;
    }
    void setErrcode(QString errCode)
    {
        mErrcode = errCode;
    }
    void setErrMessage(QString errMessage)
    {
        merrMessage = errMessage;
    }


private:
    QString mSuccess ;
    QString mErrcode;
    QString merrMessage;
};
#endif // UPDATEACCOUNTPAIDRESP_H
