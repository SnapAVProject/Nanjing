#ifndef JUDGEPWDRESP_H
#define JUDGEPWDRESP_H

#include <QObject>
#include <QString>

class judgePwdResp:public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success     WRITE setSuccess)
    Q_PROPERTY(QString errCode           READ errCode     WRITE setErrcode)
    Q_PROPERTY(QString errMessage        READ errMessage  WRITE setErrMessage)
    Q_PROPERTY(QString money             READ money       WRITE setMoney)

public:

    judgePwdResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString money()   const{return mMoney;}
    QString errCode() const{return mErrcode;}
    QString errMessage() const{return merrMessage;}

    ~judgePwdResp()
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
    void setMoney(QString money)
    {
        mMoney = money;
    }


private:
    QString mSuccess ;
    QString mMoney;
    QString mErrcode;
    QString merrMessage;
};

#endif // JUDGEPWDRESP_H
