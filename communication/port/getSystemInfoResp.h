#ifndef GETSYSTEMINFORESP_H
#define GETSYSTEMINFORESP_H


#include <QObject>

class getSystemInfoResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success             READ success            WRITE setSuccess)
    Q_PROPERTY(QString errCode             READ errCode            WRITE setErrcode)
    Q_PROPERTY(QString errMessage          READ errMessage         WRITE setErrMessage)
    Q_PROPERTY(QString systemMaintenance   READ systemMaintenance  WRITE setSystemMaintenance)
public:
    getSystemInfoResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString errMessage() const{return merrMessage;}
    QString systemMaintenance() const{return msystemMaintenance;}

    ~getSystemInfoResp()
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

    void setSystemMaintenance(QString systemMaintenance)
    {
        msystemMaintenance = systemMaintenance;
    }

private:
    QString mSuccess ;
    QString mErrcode;
    QString merrMessage;
    QString msystemMaintenance;
 };

#endif // GETSYSTEMINFORESP_H
