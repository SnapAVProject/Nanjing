#ifndef GETVIPCARDINFORESP_H
#define GETVIPCARDINFORESP_H


#include <QObject>
#include <QString>

class getVIPCardInfoResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success     WRITE setSuccess)
    Q_PROPERTY(QString errCode           READ errCode     WRITE setErrcode)
    Q_PROPERTY(QString errMessage        READ errMessage  WRITE setErrMessage)
    Q_PROPERTY(QString balance           READ balance     WRITE setBalance)
    Q_PROPERTY(QString idCard            READ idCard      WRITE setIdCard)
    Q_PROPERTY(QString m1Card            READ m1Card      WRITE setM1Card)
    Q_PROPERTY(QString mobilePhone       READ mobilePhone WRITE setMobilePhone)
    Q_PROPERTY(QString vipGrade          READ vipGrade    WRITE setVipGrade)
    Q_PROPERTY(QString vipPercent        READ vipPercent  WRITE setVipPercent)
    Q_PROPERTY(QString isExpired         READ isExpired   WRITE setIsExpired)
public:
    getVIPCardInfoResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString errMessage() const{return mErrMessage;}
    QString balance() const{return mBalance;}
    QString idCard() const{return mPidCard;}
    QString m1Card() const{return mM1Card;}
    QString mobilePhone() const{return mMobilePhone;}
    QString vipGrade() const{return mVipGrade;}
    QString vipPercent() const{return mVipPercent;}
    QString isExpired() const{return mIsExpired;}

    ~getVIPCardInfoResp()
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
        mErrMessage = errMessage;
    }

    void setBalance(QString balance)
    {
        mBalance = balance;
    }
    void setIdCard(QString paidCard)
    {
        mPidCard = paidCard;
    }
    void setM1Card(QString m1Card)
    {
        mM1Card = m1Card;
    }
    void setMobilePhone(QString mobilephone)
    {
        mMobilePhone = mobilephone;
    }
    void setVipGrade(QString vipgrade)
    {
        mVipGrade = vipgrade;
    }
    void setVipPercent(QString vipPercent)
    {
        mVipPercent = vipPercent;
    }

    void setIsExpired(QString isExpired)
    {
        mIsExpired = isExpired;
    }

private:
    QString mSuccess ;
    QString mErrcode;
    QString mErrMessage;
    QString mBalance;
    QString mPidCard;
    QString mM1Card;
    QString mMobilePhone;
    QString mVipGrade;
    QString mVipPercent;
    QString mIsExpired;
 };
#endif // GETVIPCARDINFORESP_H
