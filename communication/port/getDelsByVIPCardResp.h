#ifndef GETDELSBYVIPCARDRESP_H
#define GETDELSBYVIPCARDRESP_H

#include <QString>
#include <QObject>
#include <QList>
#include <QVariantMap>
#include "qobjecthelper.h"


class Delivery : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString deliveryId    READ  deliveryId   WRITE setDeliveryId)
    Q_PROPERTY(QString scellId       READ  scellId      WRITE setScellId)
    Q_PROPERTY(QString mobilePhone   READ  mobilePhone  WRITE setMobilePhone)
    Q_PROPERTY(QString scellStatus   READ  scellStatus  WRITE setScellStatus)
    Q_PROPERTY(QString packageId     READ  packageId    WRITE setPackageId)
    Q_PROPERTY(QString scellType     READ  scellType    WRITE setScellType)   
    Q_PROPERTY(QString debtCost      READ  debtCost     WRITE setDebtCost)
    Q_PROPERTY(QString delOwnerCost  READ  delOwnerCost WRITE setDelOwnerCost)
    Q_PROPERTY(QString ownerHasPay   READ  ownerHasPay  WRITE setOwnerHasPay)

public:

    explicit Delivery(QObject *parent = 0): QObject(parent){}

    QString deliveryId() const{return mDeliveryId;}
    QString packageId() const{return mPakeageId;}
    QString mobilePhone() const{return mMobilePhone;}
    QString scellId() const{return mCellId;}
    QString scellStatus() const{return mScellStatus;}
    QString scellType() const{return mScellType;}

    QString debtCost() const{return mDebtCost;}
    QString delOwnerCost() const{return mDelOwnerCost;}
    QString ownerHasPay() const{return mOwnerHasPay;}

    void setDeliveryId(QString deliveryId)
    {
        mDeliveryId = deliveryId;
    }

    void setScellId(QString CellId)
    {
        mCellId = CellId;
    }

    void setPackageId(QString pakeageId)
    {
        mPakeageId = pakeageId;
    }

    void setMobilePhone(QString mobilePhone)
    {
        mMobilePhone = mobilePhone;
    }

    void setScellStatus(QString scellStatus)
    {
        mScellStatus = scellStatus;
    }

    void setScellType(QString scellType)
    {
        mScellType = scellType;
    }

    void setDebtCost(QString debtCost)
    {
        mDebtCost = debtCost;
    }

    void setDelOwnerCost(QString delOwnerCost)
    {
        mDelOwnerCost = delOwnerCost;
    }

    void setOwnerHasPay(QString ownerHasPay)
    {
        mOwnerHasPay = ownerHasPay;
    }

private:
    QString mPakeageId ;
    QString mMobilePhone ;
    QString mCellId;
    QString mDeliveryId;
    QString mScellStatus;
    QString mScellType;
    QString mDebtCost;
    QString mDelOwnerCost;
    QString mOwnerHasPay;
};


class getDelsByVIPCardResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success                    READ success        WRITE setSuccess)
    Q_PROPERTY(QString errCode                    READ errCode        WRITE setErrcode)
    Q_PROPERTY(QString errMessage                 READ errMessage     WRITE setErrMessage)
    Q_PROPERTY(QString isExpired                  READ isExpired      WRITE setIsExpired)
    Q_PROPERTY(QString m1Card                     READ m1Card         WRITE setM1Card)
    Q_PROPERTY(QString vipPercent                 READ vipPercent     WRITE setVipPercent)
    Q_PROPERTY(QString balance                    READ balance        WRITE setBalance)
    Q_PROPERTY(QVariantList deliveryList          READ deliveryList   WRITE setDeliveryList)
public:
    getDelsByVIPCardResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString errMessage() const{return merrMessage;}
    QString isExpired() const{return mIsExpired;}
    QString m1Card() const{return mM1Card;}
    QString vipPercent() const{return mVipPercent;}
    QString balance() const{return mBalance;}
    QVariantList deliveryList() const
    {
       QVariantList list;
       return  list;
    }

    QList<Delivery*>getDeliveryInfo()
    {
        return mDeliveryList;
    }

    ~getDelsByVIPCardResp()
    {
        for (int i = 0; i < mDeliveryList.size(); i++)
        {
            delete mDeliveryList[i];
        }
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

    void setIsExpired(QString isExpired)
    {
        mIsExpired = isExpired;
    }

    void setM1Card(QString m1Card)
    {
        mM1Card = m1Card;
    }

    void setVipPercent(QString vipPercent)
    {
        mVipPercent = vipPercent;
    }

    void setBalance(QString balance)
    {
        mBalance = balance;
    }

    void setDeliveryList(QVariantList deliverylist)
    {
        for (int i = 0; i < deliverylist.size(); i++)
        {
            Delivery* delivery = new Delivery;
            mObjectHelper.qvariant2qobject(deliverylist[i].toMap(), delivery);
            mDeliveryList.append(delivery);
        }
    }

private:
    QString mSuccess ;
    QString mErrcode;
    QString merrMessage;
    QString mIsExpired ;
    QString mM1Card;
    QString mVipPercent;
    QString mBalance;
    QList<Delivery*> mDeliveryList;
    QJson::QObjectHelper mObjectHelper;
 };

#endif // GETDELSBYVIPCARDRESP_H
