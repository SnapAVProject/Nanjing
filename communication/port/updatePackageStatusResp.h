#ifndef UPDATEPACKAGESTATUSRESP_H
#define UPDATEPACKAGESTATUSRESP_H


#include <QObject>
#include <QString>
#include <QVariantMap>
#include "qobjecthelper.h"

class updateDelivery : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString deliveryId    READ  deliveryId   WRITE setDeliveryId)
    Q_PROPERTY(QString scabId        READ  scabId       WRITE setScabId)
    Q_PROPERTY(QString scellId       READ  scellId      WRITE setScellId)
    Q_PROPERTY(QString getCode       READ  getCode      WRITE setGetCode)
    Q_PROPERTY(QString scellType     READ  scellType    WRITE setScellType)

public:

    explicit updateDelivery(QObject *parent = 0): QObject(parent){}

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

class updatePackageStatusResp:public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success                  READ success           WRITE setSuccess)
    Q_PROPERTY(QString scellType                READ scellType         WRITE setScellType)
    Q_PROPERTY(QVariantList localDeliveries     READ localDeliveries   WRITE setLocalDeliveries)


public:

    updatePackageStatusResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString scellType() const{return mScellType;}

    QVariantList localDeliveries() const
    {
       QVariantList list;
       return  list;
    }

    QList<updateDelivery*>getDeliveryInfo()
    {
        return mDeliveryList;
    }


    ~updatePackageStatusResp()
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

    void setScellType(QString scellType)
    {
        mScellType = scellType;
    }

    void setLocalDeliveries(QVariantList deliverylist)
    {
        for (int i = 0; i < deliverylist.size(); i++)
        {
            updateDelivery* delivery = new updateDelivery;
            mObjectHelper.qvariant2qobject(deliverylist[i].toMap(), delivery);
            mDeliveryList.append(delivery);
        }
    }



private:
    QString mSuccess ;
    QString mScellType;
    QList<updateDelivery*> mDeliveryList;
    QJson::QObjectHelper mObjectHelper;

};
#endif // UPDATEPACKAGESTATUSRESP_H
