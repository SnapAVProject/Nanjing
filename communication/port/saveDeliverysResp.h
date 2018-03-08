#ifndef SAVEDELIVERYSRESP_H
#define SAVEDELIVERYSRESP_H

#include <QString>
#include <QObject>
#include <QList>
#include <QVariantMap>
#include "qobjecthelper.h"

class saveDelivery : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString deliveryId    READ  deliveryId   WRITE setDeliveryId)
    Q_PROPERTY(QString scabId        READ  scabId       WRITE setScabId)
    Q_PROPERTY(QString scellId       READ  scellId      WRITE setScellId)
    Q_PROPERTY(QString getCode       READ  getCode      WRITE setGetCode)
    Q_PROPERTY(QString scellType     READ  scellType    WRITE setScellType)

public:

    explicit saveDelivery(QObject *parent = 0): QObject(parent){}

    QString deliveryId() const{return mDeliveryId;}
    QString scabId() const{return mScabId;}

    QString scellId() const{return mCellId;}

    QString scellType() const{return mScellType;}

    QString getCode() const{return mGetCode;}


    void setDeliveryId(QString deliveryId)
    {
        mDeliveryId = deliveryId;
    }

    void setScellId(QString CellId)
    {
        mCellId = CellId;
    }

    void setScabId(QString scabId)
    {
        mScabId = scabId;
    }

    void setGetCode(QString getCode)
    {
        mGetCode = getCode;
    }

    void setScellType(QString scellType)
    {
        mScellType = scellType;
    }


private:
    QString mScabId;
    QString mCellId;
    QString mDeliveryId;
    QString mGetCode;
    QString mScellType;
};

class saveDeliverysResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success                  READ success           WRITE setSuccess)
    Q_PROPERTY(QString errCode                  READ errCode           WRITE setErrcode)
    Q_PROPERTY(QVariantList localDeliveries     READ localDeliveries   WRITE setLocalDeliveries)

public:

    saveDeliverysResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QVariantList localDeliveries() const
    {
       QVariantList list;
       return  list;
    }

    QList<saveDelivery*>getDeliveryInfo()
    {
        return mDeliveryList;
    }

    ~saveDeliverysResp()
    {
        for (int i = 0; i < mDeliveryList.size(); i++)
        {
            delete mDeliveryList[i];
        }
    }

    void setSuccess(QString success)
    {
        mSuccess = success;
    }

    void setErrcode(QString errCode)
    {
        mErrcode = errCode;
    }

    void setLocalDeliveries(QVariantList deliverylist)
    {
        for (int i = 0; i < deliverylist.size(); i++)
        {
            saveDelivery* delivery = new saveDelivery;
            mObjectHelper.qvariant2qobject(deliverylist[i].toMap(), delivery);
            mDeliveryList.append(delivery);
        }
    }

private:
    QString mSuccess ;  //快递员存件保存信息的返回值
    QString mErrcode;
    QList<saveDelivery*> mDeliveryList;
    QJson::QObjectHelper mObjectHelper;
};
#endif // SAVEDELIVERYSRESP_H
