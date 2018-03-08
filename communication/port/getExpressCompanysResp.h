#ifndef GETEXPRESSCOMPANYSRESP_H
#define GETEXPRESSCOMPANYSRESP_H

#include <QObject>
#include <QString>
#include <QList>
#include <QVariantList>

#include "qobjecthelper.h"


class Company : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString companyId   READ  companyId    WRITE setCompanyId)
    Q_PROPERTY(QString companyName READ  companyName  WRITE setCompanyName)
public:

    explicit Company(QObject *parent = 0): QObject(parent){}

    QString companyId() const{return mCompanyId;}
    QString companyName() const{return mCompanyName;}

    void setCompanyId(QString companyId)
    {
        mCompanyId = companyId;
    }

    void setCompanyName(QString companyName)
    {
        mCompanyName = companyName;
    }
private:
    QString mCompanyId ;
    QString mCompanyName ;
};



class getExpressCompanysResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success     WRITE setSuccess)
    Q_PROPERTY(QString errCode           READ errCode     WRITE setErrcode)
    Q_PROPERTY(QVariantList companyList  READ companyList WRITE setConfirmList)
public:

    getExpressCompanysResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QVariantList companyList() const
    {
        QVariantList list;
        return list;
    }

    ~getExpressCompanysResp()
    {
        for (int i = 0; i < mCompanyList.size(); i++)
        {
            delete mCompanyList[i];
        }
    }

    void setSuccess(QString success)
    {
        mSuccess = success;
    }

    void setErrcode(QString errCode)
    {
        mErrcode = errCode;
    }


    void setConfirmList(QVariantList companyList)
    {


        for (int i = 0; i < companyList.size(); i++)
        {
            Company* company = new Company;
            mObjectHelper.qvariant2qobject(companyList[i].toMap(), company);
            mCompanyList.append(company);
        }
    }

private:
    QString mSuccess ;
    QString mErrcode;
    QList<Company*> mCompanyList;
    QJson::QObjectHelper mObjectHelper;
};
#endif // GETEXPRESSCOMPANYSRESP_H
