#ifndef CHECKOPENSCELLRESP_H
#define CHECKOPENSCELLRESP_H

#include <QObject>
#include <QString>

class checkOpenScellResp : public QObject
{
    Q_OBJECT


    Q_PROPERTY(QString errCode              READ  errCode          WRITE setErrcode)
    Q_PROPERTY(QString errMessage           READ  errMessage       WRITE setErrMessage)

public:

    explicit checkOpenScellResp(QObject *parent = 0): QObject(parent){}

    QString errCode() const{return mResultCode;}
    QString errMessage() const{return mResultMessage;}

    void setErrcode(QString resultCode)
    {
        mResultCode = resultCode;
    }

    void setErrMessage(QString resultMessage)
    {
        mResultMessage = resultMessage;
    }

private:

    QString mResultCode;
    QString mResultMessage;

};
#endif // CHECKOPENSCELLRESP_H
