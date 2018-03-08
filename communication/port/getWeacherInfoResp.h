#ifndef GETWEACHERINFORESP_H
#define GETWEACHERINFORESP_H


#include <QObject>
#include <QStringList>

class getWeacherInfoResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success     WRITE setSuccess)
    Q_PROPERTY(QString errCode           READ errCode     WRITE setErrcode)
    Q_PROPERTY(QString errMessage        READ errMessage  WRITE setErrMessage)
    Q_PROPERTY(QString city              READ city        WRITE setCity)
    Q_PROPERTY(QString temperature       READ temperature WRITE setTemperature)
    Q_PROPERTY(QString weacher           READ weacher     WRITE setWeacher)
    Q_PROPERTY(QString wind              READ wind        WRITE setWind)

public:
    getWeacherInfoResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString errMessage() const{return merrMessage;}
    QString city() const{return mCity;}
    QString temperature() const{return mTemperature;}
    QString weacher() const{return mWeacher;}
    QString wind() const{return mWind;}


    ~getWeacherInfoResp()
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

    void setCity(QString city)
    {
        mCity = city;
    }

    void setTemperature(QString temperature)
    {
        mTemperature = temperature;
    }

    void setWeacher(QString weacher)
    {
        mWeacher = weacher;
    }

    void setWind(QString wind)
    {
        mWind = wind;
    }

private:
    QString mSuccess ;
    QString mErrcode;
    QString merrMessage;
    QString mCity;
    QString mTemperature;
    QString mWeacher;
    QString mWind;
 };

#endif // GETWEACHERINFORESP_H
