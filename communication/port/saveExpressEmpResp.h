#ifndef SAVEEXPRESSEMPRESP_H
#define SAVEEXPRESSEMPRESP_H

#include <QObject>
#include <QString>


class saveExpressEmpResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success     WRITE setSuccess)
    Q_PROPERTY(QString errCode           READ errCode     WRITE setErrcode)

public:

    saveExpressEmpResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}

    ~saveExpressEmpResp()
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

private:
    QString mSuccess ;  //录入快递员信息的返回值
    QString mErrcode;
};
#endif // SAVEEXPRESSEMPRESP_H
