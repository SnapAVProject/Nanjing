#ifndef REPAIRCELLERRORSRESP_H
#define REPAIRCELLERRORSRESP_H

#include <QObject>
#include <QString>

class repairCellErrorsResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success     WRITE setSuccess)

public:

    repairCellErrorsResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}

    ~repairCellErrorsResp()
    {
    }

    void setSuccess(QString success)
    {
        mSuccess = success;
    }

private:
    QString mSuccess ;  //异常格子修复上报
};

#endif // REPAIRCELLERRORSRESP_H
