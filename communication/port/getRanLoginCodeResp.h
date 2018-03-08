#ifndef GETRANLOGINCODERESP_H
#define GETRANLOGINCODERESP_H
#include <QObject>
#include <QString>


class getRanLoginCodeResp : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString success           READ success     WRITE setSuccess)
    Q_PROPERTY(QString errCode           READ errCode     WRITE setErrcode)
    Q_PROPERTY(QString errMessage        READ errMessage  WRITE setErrMessage)
    Q_PROPERTY(QString messType          READ messType    WRITE setMessType)

public:

    getRanLoginCodeResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString errMessage() const{return merrMessage;}
    QString messType() const{return mmessType;}

    ~getRanLoginCodeResp()
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

    void setErrMessage(QString errMessage)
    {
        merrMessage = errMessage;
    }

    void setMessType(QString messType)
    {
        mmessType = messType;
    }

private:
    QString mSuccess ;  //快递员取得登录密码返回值
    QString mErrcode;
    QString merrMessage;
    QString mmessType;
};

#endif // GETRANLOGINCODERESP_H
