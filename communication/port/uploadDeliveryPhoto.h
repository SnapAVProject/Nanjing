#ifndef UPLOADDELIVERYPHOTO_H
#define UPLOADDELIVERYPHOTO_H

#include <QObject>
#include <QString>

class uploadDeliveryPhotoResp:public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success     WRITE setSuccess)

public:

    uploadDeliveryPhotoResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}


    ~uploadDeliveryPhotoResp()
    {
    }

    void setSuccess(QString success)
    {
        mSuccess = success;
    }



private:
    QString mSuccess ;

};

#endif // UPLOADDELIVERYPHOTO_H
