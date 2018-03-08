#ifndef GETDELIVERYBYCODERESP_H
#define GETDELIVERYBYCODERESP_H

#include <QObject>
#include <QString>

class getDeliveryByCodeResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success       WRITE setSuccess)
    Q_PROPERTY(QString errCode           READ errCode       WRITE setErrcode)
    Q_PROPERTY(QString scellId           READ cellId        WRITE setCellId)
    Q_PROPERTY(QString debtCost          READ debt          WRITE setDebt)  //滞留费多少
    Q_PROPERTY(QString delOwnerCost      READ delOwnerCost  WRITE setDelOwnerCost)  //业主需要付费多少
    Q_PROPERTY(QString ownerHasPay       READ ownerHasPay   WRITE setOwnerHasPay)  //用户已支付了多少钱
    Q_PROPERTY(QString deliveryId        READ deliveryId    WRITE setDeliveryId)
    Q_PROPERTY(QString errMessage        READ errMessage    WRITE setErrMessage)
    Q_PROPERTY(QString scellType         READ scellType     WRITE setScellType)
    Q_PROPERTY(QString vipPercent        READ vipPercent    WRITE setVipPercent)
    Q_PROPERTY(QString mobilePhone       READ mobilePhone   WRITE setMobilePhone)

public:

    getDeliveryByCodeResp(QObject *parent = 0): QObject(parent){ mdebt="0"; }

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString cellId() const{return mcellId;}
    QString deliveryId() const{return mdeliveryId;}
    QString debt() const{return mdebt;}
    QString delOwnerCost() const{return mOwndebt;}
    QString ownerHasPay() const{return mPaid;}
    QString errMessage() const{return merrMessage;}
    QString scellType() const{return mScelltype;}
    QString vipPercent() const{return mVipPercent;}
    QString mobilePhone() const{return mMobilePhone;}

    ~getDeliveryByCodeResp()
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

    void setCellId(QString cellId)
    {
        mcellId = cellId;
    }

    void setDebt(QString debt)
    {
        mdebt = debt;
    }

    void setDelOwnerCost(QString owndebt)
    {
        mOwndebt = owndebt;
    }

    void setOwnerHasPay(QString paid)
    {
        mPaid = paid;
    }

    void setDeliveryId(QString deliveryId)
    {
        mdeliveryId = deliveryId;
    }

    void setErrMessage(QString errMessage)
    {
        merrMessage = errMessage;
    }

    void setScellType(QString scelltype)
    {
        mScelltype = scelltype;
    }

    void setVipPercent(QString vipPercent)
    {
        mVipPercent = vipPercent;
    }

    void setMobilePhone(QString mobilePhone)
    {
        mMobilePhone = mobilePhone;
    }

private:
    QString mSuccess;    //用户输入取件码的返回值
    QString mErrcode;
    QString mcellId;     //箱子号scellId
    QString mdeliveryId; //派件编号
    QString mdebt;       //滞留欠费
    QString mOwndebt;  //用户需付费用
    QString mPaid;     //已付费用
    QString merrMessage;
    QString mScelltype;
    QString mVipPercent;
    QString mMobilePhone;
};
#endif // GETDELIVERYBYCODERESP_H
