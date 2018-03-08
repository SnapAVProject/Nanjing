#ifndef EXPGETSAVEDELIVERYRESP_H
#define EXPGETSAVEDELIVERYRESP_H



#include <QObject>
#include <QStringList>
#include <QList>
#include <QVariantMap>
#include "qobjecthelper.h"

class takeDeliveryInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString deliveryId   READ  deliveryId    WRITE setDeliveryId)
    Q_PROPERTY(QString scellId      READ  scellId       WRITE setScellId)
    Q_PROPERTY(QString packageId    READ  packageId     WRITE setPackageId)
    Q_PROPERTY(QString createTime   READ  createTime    WRITE setCreateTime)
    Q_PROPERTY(QString scabId       READ  scabId        WRITE setScabId)
    Q_PROPERTY(QString scellType    READ  scellType     WRITE setScellType)
    Q_PROPERTY(QString mobilePhone  READ  mobilePhone   WRITE setMobilePhone)

public:

    explicit takeDeliveryInfo(QObject *parent = 0): QObject(parent){}

    QString deliveryId() const{return mDeliveryId;}
    QString scellId() const{return mScellId;}
    QString packageId() const{return mPackageId;}
    QString createTime() const{return mCreateTime;}
    QString scabId() const{return mScabId;}
    QString scellType() const{return mScellType;}
    QString mobilePhone() const{return mMobilePhone;}

    void setDeliveryId(QString deliveryId)
    {
        mDeliveryId = deliveryId;
    }

    void setScellId(QString scellId)
    {
        mScellId = scellId;
    }

    void setPackageId(QString packageId)
    {
        mPackageId = packageId;
    }

    void setCreateTime(QString createTime)
    {
        mCreateTime = createTime;
    }

    void setScabId(QString scabId)
    {
        mScabId = scabId;
    }

    void setScellType(QString scellType)
    {
        mScellType = scellType;
    }

    void setMobilePhone(QString mobilePhone)
    {
        mMobilePhone = mobilePhone;
    }
private:
    QString mDeliveryId ;
    QString mScellId;
    QString mPackageId;
    QString mCreateTime;
    QString mScabId;
    QString mScellType;
    QString mMobilePhone;
};


class expGetSaveDeliveryResp : public QObject
{
    Q_OBJECT


    Q_PROPERTY(QString errCode              READ  errCode          WRITE setErrcode)
    Q_PROPERTY(QString errMessage           READ  errMessage       WRITE setErrMessage)
    Q_PROPERTY(QVariantList deliveryList    READ  deliveryList     WRITE setDeliveryList)

public:

    explicit expGetSaveDeliveryResp(QObject *parent = 0): QObject(parent){}

    QString errCode() const{return mResultCode;}
    QString errMessage() const{return mResultMessage;}

    QVariantList deliveryList() const
    {
        QVariantList list;
        return  list;
    }

    QList<takeDeliveryInfo*>getDeliveryInfoList()
    {
        return mDeliveryList;
    }

    ~expGetSaveDeliveryResp()
    {
        for (int i = 0; i < mDeliveryList.size(); i++)
        {
            delete mDeliveryList[i];
        }
    }

    void setDeliveryList(QVariantList deliverylist)
    {
        for (int i = 0; i < deliverylist.size(); i++)
        {
            takeDeliveryInfo* delivery = new takeDeliveryInfo;
            mObjectHelper.qvariant2qobject(deliverylist[i].toMap(), delivery);
            mDeliveryList.append(delivery);
        }
    }

    void setErrcode(QString resultCode)
    {
        mResultCode = resultCode;
    }

    void setErrMessage(QString resultMessage)
    {
        mResultMessage = resultMessage;
    }

private:

    QList<takeDeliveryInfo*> mDeliveryList;
    QString mResultCode;
    QString mResultMessage;
    QJson::QObjectHelper mObjectHelper;
};
#endif // EXPGETSAVEDELIVERYRESP_H
