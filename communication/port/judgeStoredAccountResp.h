#ifndef JUDGESTOREDACCOUNTRESP_H
#define JUDGESTOREDACCOUNTRESP_H

#include <QObject>
#include <QString>

class judgeStoredAccountResp:public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success               READ success         WRITE setSuccess)
    Q_PROPERTY(QString errCode               READ errCode         WRITE setErrCode)
    Q_PROPERTY(QString empName               READ empName         WRITE setEmpName)
    Q_PROPERTY(QString m1Card                READ m1Card          WRITE setM1Card)
    Q_PROPERTY(QString mobilePhone           READ mobilePhone     WRITE setMobilePhone)
    Q_PROPERTY(QString balance               READ balance         WRITE setBalance)
    Q_PROPERTY(QString price                 READ price           WRITE setPrice)
    Q_PROPERTY(QString cardType              READ cardType        WRITE setCardType)
    Q_PROPERTY(QString cardTimeStart         READ cardTimeStart   WRITE setCardTimeStart)
    Q_PROPERTY(QString cardTimeEnd           READ cardTimeEnd     WRITE setCardTimeEnd)
    Q_PROPERTY(QString topTimeEnd            READ topTimeEnd      WRITE setTopTimeEnd)
    Q_PROPERTY(QString cardCycle             READ cardCycle       WRITE setCardCycle)

public:

    judgeStoredAccountResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrCode;}
    QString empName() const{return mEmpName;}
    QString m1Card()  const{return mM1Card;}
    QString mobilePhone() const{return mMobilePhone;}
    QString balance() const{return mBalance;}
    QString price() const{return mPrice;}
    QString cardType() const{return mCardType;}
    QString cardTimeStart() const{return mCardTimeStart;}
    QString cardTimeEnd() const{return mCardTimeEnd;}
    QString topTimeEnd() const{return mTopTimeEnd;}
    QString cardCycle() const{return mCardCycle;}



    ~judgeStoredAccountResp()
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
    void setEmpName(QString empName)
    {
        mEmpName = empName;
    }
    void setM1Card(QString m1Card)
    {
        mM1Card = m1Card;
    }
    void setMobilePhone(QString mobilePhone)
    {
        mMobilePhone = mobilePhone;
    }
    void setBalance(QString balance)
    {
        mBalance = balance;
    }
    void setPrice(QString price)
    {
        mPrice = price;
    }

    void setCardType(QString cardType)
    {
        mCardType = cardType;
    }
    void setCardTimeStart(QString cardTimeStart)
    {
        mCardTimeStart = cardTimeStart;
    }
    void setCardTimeEnd(QString cardTimeEnd)
    {
        mCardTimeEnd = cardTimeEnd;
    }
    void setTopTimeEnd(QString topTimeEnd)
    {
        mTopTimeEnd = topTimeEnd;
    }
    void setCardCycle(QString cardCycle)
    {
        mCardCycle = cardCycle;
    }


private:
    QString mSuccess ;
    QString mErrCode;
    QString mEmpName;
    QString mM1Card;
    QString mMobilePhone;
    QString mBalance;
    QString mPrice;
    QString mCardType;
    QString mCardTimeStart;
    QString mCardTimeEnd;
    QString mTopTimeEnd;
    QString mCardCycle;
};
#endif // JUDGESTOREDACCOUNTRESP_H
