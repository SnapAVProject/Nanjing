#ifndef GETDELIVERYCONFIRMRESP_H
#define GETDELIVERYCONFIRMRESP_H

#include <QObject>
#include <QString>

class getDeliveryConfirmResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success     WRITE setSuccess)
    Q_PROPERTY(QString errCode           READ errCode     WRITE setErrcode)
    Q_PROPERTY(QString errMessage        READ errMessage  WRITE setErrMessage)
    Q_PROPERTY(QString scellType         READ scellType   WRITE setScellType)
public:
    getDeliveryConfirmResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString errMessage() const{return merrMessage;}
    QString scellType() const{return mScelltype;}

    ~getDeliveryConfirmResp()
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

    void setScellType(QString scelltype)
    {
        mScelltype = scelltype;
    }

private:
    QString mSuccess ;
    QString mErrcode;
    QString merrMessage;
    QString mScelltype;
 };
#endif // GETDELIVERYCONFIRMRESP_H
