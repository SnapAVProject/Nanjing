#ifndef RECHARGETOACCOUNTRESP_H
#define RECHARGETOACCOUNTRESP_H

#include <QObject>
#include <QString>

class rechargeToAccountResp:public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success               READ success         WRITE setSuccess)
    Q_PROPERTY(QString errCode               READ errCode         WRITE setErrCode)
    Q_PROPERTY(QString balance               READ balance         WRITE setBalance)
    Q_PROPERTY(QString price                 READ price           WRITE setPrice)

public:

    rechargeToAccountResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrCode;}
    QString balance() const{return mBalance;}
    QString price() const{return mPrice;}


    ~rechargeToAccountResp()
    {
    }

    void setSuccess(QString success)
    {
        mSuccess = success;
    }
    void setErrCode(QString errCode)
    {
        mErrCode = errCode;
    }
    void setBalance(QString balance)
    {
        mBalance = balance;
    }
    void setPrice(QString price)
    {
        mPrice = price;
    }



private:
    QString mSuccess ;
    QString mErrCode;
    QString mBalance;
    QString mPrice;
};
#endif // RECHARGETOACCOUNTRESP_H
