#ifndef CHECKBALANCERESP_H
#define CHECKBALANCERESP_H

#include <QObject>
#include <QStringList>

class checkBalanceResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success         WRITE setSuccess)
    Q_PROPERTY(QString errCode           READ errCode         WRITE setErrcode)
    Q_PROPERTY(QString errMessage        READ errMessage      WRITE setErrMessage)
    Q_PROPERTY(QString balance           READ balance         WRITE setBalance)
    Q_PROPERTY(QString cardTimeStart     READ cardTimeStart   WRITE setCardTimeStart)
    Q_PROPERTY(QString cardTimeEnd       READ cardTimeEnd     WRITE setCardTimeEnd)
public:
    checkBalanceResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString errMessage() const{return merrMessage;}
    QString balance() const{return mBalance;}
    QString cardTimeStart() const{return mCardTimeStart;}
    QString cardTimeEnd() const{return mCardTimeEnd;}

    ~checkBalanceResp()
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

    void setBalance(QString balance)
    {
        mBalance = balance;
    }

    void setCardTimeStart(QString cardTimeStart)
    {
        mCardTimeStart = cardTimeStart;
    }
    void setCardTimeEnd(QString cardTimeEnd)
    {
        mCardTimeEnd = cardTimeEnd;
    }

private:
    QString mSuccess ;
    QString mErrcode;
    QString merrMessage;
    QString mBalance;
    QString mCardTimeStart;
    QString mCardTimeEnd;
 };
#endif // CHECKBALANCERESP_H
