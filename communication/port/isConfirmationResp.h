#ifndef ISCONFIRMATIONRESP_H
#define ISCONFIRMATIONRESP_H

#include <QObject>
#include <QString>

class isConfirmationResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success     WRITE setSuccess)
    Q_PROPERTY(QString errCode           READ errCode     WRITE setErrcode)
    Q_PROPERTY(QString errMessage        READ errMessage  WRITE setErrMessage)
    Q_PROPERTY(QString creatTime         READ creatTime   WRITE setCreatTime)
    Q_PROPERTY(QString md5No             READ md5No       WRITE setMd5No)
    Q_PROPERTY(QString systemDate        READ systemDate  WRITE setSystemDate)

public:
    isConfirmationResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString errMessage() const{return merrMessage;}
    QString creatTime() const{return mCreatTime;}
    QString md5No() const{return mMd5No;}
    QString systemDate() const{return mDate;}

    ~isConfirmationResp()
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

    void setCreatTime(QString creatTime)
    {
        mCreatTime = creatTime;
    }

    void setMd5No(QString md5No)
    {
        mMd5No = md5No;
    }

    void setSystemDate(QString date)
    {
        mDate = date;
    }


private:
    QString mSuccess ;
    QString mErrcode;
    QString merrMessage;
    QString mCreatTime ;
    QString mMd5No;
    QString mDate;
 };
#endif // ISCONFIRMATIONRESP_H
