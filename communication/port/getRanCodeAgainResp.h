#ifndef GETRANCODEAGAINRESP_H
#define GETRANCODEAGAINRESP_H

#include <QObject>
#include <QString>


class getRanCodeAgainResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success     WRITE setSuccess)
    Q_PROPERTY(QString errCode           READ errCode     WRITE setErrcode)
    Q_PROPERTY(QString errMessage        READ errMessage  WRITE setErrMessage)
    Q_PROPERTY(QString messType          READ messType    WRITE setMessType)

public:

    getRanCodeAgainResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString errMessage() const{return merrMessage;}
    QString messType() const{return mmessType;} //1、短信、APP全发 2、只发短信 3、只发APP

    ~getRanCodeAgainResp()
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
    QString mSuccess ;  //用户取件获得取件码的返回值
    QString mErrcode;
    QString merrMessage;
    QString mmessType;
};
#endif // GETRANCODEAGAINRESP_H
