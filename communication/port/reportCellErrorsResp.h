#ifndef REPORTCELLERRORSRESP_H
#define REPORTCELLERRORSRESP_H

#include <QObject>
#include <QString>

class reportCellErrorsResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success     WRITE setSuccess)

public:

    reportCellErrorsResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}

    ~reportCellErrorsResp()
    {
    }

    void setSuccess(QString success)
    {
        mSuccess = success;
    }

private:
    QString mSuccess ;  //异常格子上报
};

#endif // REPORTCELLERRORSRESP_H
