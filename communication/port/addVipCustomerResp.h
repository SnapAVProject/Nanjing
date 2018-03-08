#ifndef ADDVIPCUSTOMERRESP_H
#define ADDVIPCUSTOMERRESP_H

#include <QObject>
#include <QStringList>

class addVipCustomerResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success     WRITE setSuccess)
    Q_PROPERTY(QString errCode           READ errCode     WRITE setErrcode)
    Q_PROPERTY(QString errMessage        READ errMessage  WRITE setErrMessage)
public:
    addVipCustomerResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString errMessage() const{return merrMessage;}

    ~addVipCustomerResp()
    {
    }

    void setErrcode(QString errCode)
    {
        mErrcode = errCode;
    }

    void setSuccess(QString success)
    {
        mSuccess = success;
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

#endif // ADDVIPCUSTOMERRESP_H
