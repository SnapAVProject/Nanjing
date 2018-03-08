#ifndef M1CARDLOGINRESP_H
#define M1CARDLOGINRESP_H

#include <QDebug>
#include <QObject>
#include <QString>
#include <QList>
#include <QVariantList>
#include <QVariantMap>
#include "qobjecthelper.h"


class Confirm : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString deliveryId    READ  deliveryId   WRITE setDeliveryId)
    Q_PROPERTY(QString packageId     READ  packageId    WRITE setPakeage)
    Q_PROPERTY(QString mobilePhone   READ  mobilePhone  WRITE setMobilePhone)
    Q_PROPERTY(QString scellId       READ  scellId      WRITE setCellId)

public:

    explicit Confirm(QObject *parent = 0): QObject(parent){}

    QString deliveryId() const{return mDeliveryId;}
    QString packageId() const{return mPakeageId;}
    QString mobilePhone() const{return mMobilePhone;}
    QString scellId() const{return mCellId;}

    void setDeliveryId(QString deliveryId)
    {
        mDeliveryId = deliveryId;
    }

    void setPakeage(QString pakeageId)
    {
        mPakeageId = pakeageId;
    }

    void setMobilePhone(QString mobilePhone)
    {
        mMobilePhone = mobilePhone;
    }

    void setCellId(QString CellId)
    {
        mCellId = CellId;
    }

private:
    QString mPakeageId ;
    QString mMobilePhone ;
    QString mCellId;
    QString mDeliveryId;
};


class ExpressCompany : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString companyId   READ  companyId    WRITE setCompanyId)
    Q_PROPERTY(QString companyName READ  companyName  WRITE setCompanyName)
public:

    explicit ExpressCompany(QObject *parent = 0): QObject(parent){}

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


class m1CardLoginResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success                    READ success               WRITE setSuccess)
    Q_PROPERTY(QString errCode                    READ errCode               WRITE setErrcode)
    Q_PROPERTY(QString errMessage                 READ errMessage            WRITE setErrMessage)
    Q_PROPERTY(QString m1CardId                   READ m1CardId              WRITE setM1CardId)
    Q_PROPERTY(QString cardType                   READ cardType              WRITE setCardType)
    Q_PROPERTY(QString mobilePhone                READ mobilePhone           WRITE setMobilePhone)
    Q_PROPERTY(QStringList bigCellIdList          READ bigCellIdList         WRITE setbigCellIdList)
    Q_PROPERTY(QStringList middleCellIdList       READ middleCellIdList      WRITE setmiddleCellIdList)
    Q_PROPERTY(QStringList smallCellIdList        READ smallCellIdList       WRITE setsmallCellIdList)
    Q_PROPERTY(QStringList vegetablesCellidList   READ vegetablesCellidList  WRITE setvegetablesCellidList)
    Q_PROPERTY(QStringList orderBigCells          READ orderBigCells         WRITE setorderBigCells)
    Q_PROPERTY(QStringList orderMiddleCells       READ orderMiddleCells      WRITE setorderMiddleCells)
    Q_PROPERTY(QStringList orderSmallCells        READ orderSmallCells       WRITE setorderSmallCells)
    Q_PROPERTY(QStringList orderVegeCells         READ orderVegeCells        WRITE setorderVegeCells)
    Q_PROPERTY(QStringList orderCells             READ orderCells            WRITE setorderCells)
    Q_PROPERTY(QString balance                    READ balance               WRITE setBalance)
    Q_PROPERTY(QString tariffType                 READ tariffType            WRITE setTariffType)
    Q_PROPERTY(QVariantList confirmList           READ confirmList           WRITE setConfirmList)
    Q_PROPERTY(QVariantList expressCompanyList    READ companyList           WRITE setCompanyList)
    Q_PROPERTY(QStringList blackNameList          READ blackNameList         WRITE setBlackNameList)

public:

    m1CardLoginResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString m1CardId() const{return mM1CardId;}
    QString errMessage() const{return merrMessage;}
    QString cardType() const{return mCardType;}
    QString mobilePhone() const{return mMobilePhone;}
    QStringList bigCellIdList() const{return mbigCellIdList;}
    QStringList middleCellIdList() const{return mmiddleCellIdList;}
    QStringList smallCellIdList() const{return msmallCellIdList; }
    QStringList vegetablesCellidList() const{return mvegetablesCellidList;}
    QStringList orderBigCells() const{return morderBigCells;}
    QStringList orderMiddleCells() const{return morderMiddleCells;}
    QStringList orderSmallCells() const{return morderSmallCells; }
    QStringList orderVegeCells() const{return morderVegeCells;}
    QStringList orderCells() const{return morderCells;}
    QString balance() const{return mBalance;}
    QString tariffType() const{return mTariffType;}
    QVariantList confirmList() const
    {
        
        QVariantList list;
        QVariantMap map;
        for(int i = 0;i < mConfirmList.size();i++)
        {
            map.insert(mConfirmList.at(i)->packageId(),mConfirmList.at(i)->mobilePhone());
            list.append(map);
        }
        return list;
    }

    QVariantList companyList() const
    {

        QVariantList list;
        return list;
    }

    QStringList blackNameList() const{return mblackNameList;}

    ~m1CardLoginResp()
    {
        for (int i = 0; i < mConfirmList.size(); i++)
        {
            delete mConfirmList[i];
        }
        for (int i = 0; i < mAddrCompanyList.size(); i++)
        {
            delete mAddrCompanyList[i];
        }
    }
    QList<Confirm*>getArrayNew()
    {
        return mConfirmList;
    }

    QStringList getCompanyIdArrayNew()
    {
        return mCompanyIdList;
    }

    QStringList getCompanyNameArrayNew()
    {
        return mCompanyNameList;
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

    void setM1CardId(QString m1CardId)
    {
        mM1CardId = m1CardId;
    }

    void setCardType(QString cardType)
    {
        mCardType = cardType;
    }

    void setMobilePhone(QString mobilePhone)
    {
        mMobilePhone = mobilePhone;
    }

    void setbigCellIdList(QStringList bigCellIdList)
    {
        for(int i = 0;i < bigCellIdList.size(); i++)
        {
            mbigCellIdList.append(bigCellIdList.at(i));
        }
    }

    void setmiddleCellIdList(QStringList middleCellIdList)
    {
        for(int i = 0;i < middleCellIdList.size();i++)
        {
             mmiddleCellIdList.append(middleCellIdList.at(i));
        }
    }

    void setsmallCellIdList(QStringList smallCellIdList)
    {
        for (int i = 0; i < smallCellIdList.size(); i++)
        {
            msmallCellIdList.append(smallCellIdList.at(i));
        }
    }

    void setvegetablesCellidList(QStringList vegetablesCellidList)
    {
        for (int i = 0; i < vegetablesCellidList.size(); i++)
        {
            mvegetablesCellidList.append(vegetablesCellidList.at(i));
        }
    }

    void setorderBigCells(QStringList bigCellIdList)
    {
        for(int i = 0;i < bigCellIdList.size(); i++)
        {
            morderBigCells.append(bigCellIdList.at(i));
        }
    }

    void setorderMiddleCells(QStringList middleCellIdList)
    {
        for(int i = 0;i < middleCellIdList.size();i++)
        {
             morderMiddleCells.append(middleCellIdList.at(i));
        }
    }

    void setorderSmallCells(QStringList smallCellIdList)
    {
        for (int i = 0; i < smallCellIdList.size(); i++)
        {
            morderSmallCells.append(smallCellIdList.at(i));
        }
    }

    void setorderVegeCells(QStringList vegetablesCellidList)
    {
        for (int i = 0; i < vegetablesCellidList.size(); i++)
        {
            morderVegeCells.append(vegetablesCellidList.at(i));
        }
    }

    void setorderCells(QStringList orderCellList)
    {
        for (int i = 0; i < orderCellList.size(); i++)
        {
            morderCells.append(orderCellList.at(i));
        }
    }

    void setBalance(QString balance)
    {
        mBalance = balance;
    }

    void setTariffType(QString tariffType)
    {
        mTariffType = tariffType;
    }

    void setConfirmList(QVariantList confirmList)
    {


        for (int i = 0; i < confirmList.size(); i++)
        {
            Confirm* confirm = new Confirm;
            mObjectHelper.qvariant2qobject(confirmList[i].toMap(), confirm);
            mConfirmList.append(confirm);
        }
    }

    void setCompanyList(QVariantList companyList)
    {


        for (int i = 0; i < companyList.size(); i++)
        {
            ExpressCompany* company = new ExpressCompany;
            mObjectHelper.qvariant2qobject(companyList[i].toMap(), company);
            mAddrCompanyList.append(company);
            mCompanyIdList.append(company->companyId());
            mCompanyNameList.append(company->companyName());
        }

    }

    void setBlackNameList(QStringList blackNameList)
    {
        for (int i = 0; i < blackNameList.size(); i++)
        {
            mblackNameList.append(blackNameList.at(i));
        }
    }

private:
    QString mSuccess ;
    QString mM1CardId ;
    QString mErrcode;
    QString merrMessage;
    QString mCardType ;
    QList<Confirm*> mConfirmList;
    QList<ExpressCompany*> mAddrCompanyList;
    QStringList mCompanyIdList;
    QStringList mCompanyNameList;
    QStringList mbigCellIdList ;
    QStringList mmiddleCellIdList ;
    QStringList msmallCellIdList;
    QStringList mvegetablesCellidList;
    QStringList morderBigCells ;
    QStringList morderMiddleCells ;
    QStringList morderSmallCells;
    QStringList morderVegeCells;
    QStringList morderCells;
    QString mBalance;
    QString mTariffType;
    QJson::QObjectHelper mObjectHelper;
    QStringList mblackNameList;
    QString mMobilePhone;
};

#endif // M1CARDLOGINRESP_H
