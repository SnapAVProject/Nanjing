#ifndef NOM1CARDLOGINRESP_H
#define NOM1CARDLOGINRESP_H

#include <QObject>
#include <QString>
#include <QList>
#include <QVariantList>
#include "qobjecthelper.h"

class ConFirm : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString packageId    READ  packageId    WRITE setPakeage)
    Q_PROPERTY(QString mobilePhone  READ  mobilePhone  WRITE setMobilePhone)
    Q_PROPERTY(QString scellId      READ  scellId      WRITE setCellId)
    Q_PROPERTY(QString deliveryId   READ  deliveryId   WRITE setDeliveryId)
public:

    explicit ConFirm(QObject *parent = 0): QObject(parent){}

    QString packageId() const{return mPakeageId;}
    QString mobilePhone() const{return mMobilePhone;}
    QString scellId() const{return mCellId;}
    QString deliveryId() const{return mDeliveryId;}

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

    void setDeliveryId(QString deliveryId)
    {
        mDeliveryId = deliveryId;
    }
private:
    QString mPakeageId ;
    QString mMobilePhone ;
    QString mCellId;
    QString mDeliveryId;
};


class expressCompany : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString companyId   READ  companyId    WRITE setCompanyId)
    Q_PROPERTY(QString companyName READ  companyName  WRITE setCompanyName)
public:

    explicit expressCompany(QObject *parent = 0): QObject(parent){}

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



class noM1CardLoginResp : public QObject
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
    Q_PROPERTY(QVariantList confirmList           READ confirmList           WRITE setConfirmList)
    Q_PROPERTY(QVariantList expressCompanyList    READ companyList           WRITE setCompanyList)
    Q_PROPERTY(QStringList blackNameList          READ blackNameList         WRITE setBlackNameList)
public:

    noM1CardLoginResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString errMessage() const{return merrMessage;}
    QString m1CardId() const{return mM1CardId;}
    QString cardType() const{return mCardType;}
    QString mobilePhone() const{return mMobilePhone;}
    QStringList bigCellIdList() const{return mbigCellIdList;}
    QStringList middleCellIdList() const{return mmiddleCellIdList;}
    QStringList smallCellIdList() const{return msmallCellIdList; }
    QStringList vegetablesCellidList() const{return mvegetablesCellidList; }

    QVariantList confirmList() const
    {
        QVariantList list;
        return list;
    }

    QVariantList companyList() const
    {

        QVariantList list;
        return list;
    }

    QStringList blackNameList() const{return mblackNameList;}

    ~noM1CardLoginResp()
    {
        for (int i = 0; i < mConfirmList.size(); i++)
        {
            delete mConfirmList[i];
        }
        for (int i = 0; i < mCompanyList.size(); i++)
        {
            delete mCompanyList[i];
        }
    }

    QList<ConFirm*>getArrayNew()
    {
        return mConfirmList;
    }
    QList<expressCompany*>getCompanyArrayNew()
    {
        return mCompanyList;
    }

    void setSuccess(QString success)
    {
        mSuccess = success;
    }

    void setErrcode(QString errCode)
    {
        mErrcode = errCode;
    }

    void setM1CardId(QString m1CardId)
    {
        mM1CardId = m1CardId;
    }

    void setCardType(QString cardType)
    {
        mCardType = cardType;
    }

    void setErrMessage(QString errMessage)
    {
        merrMessage = errMessage;
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

    void setConfirmList(QVariantList confirmList)
    {


        for (int i = 0; i < confirmList.size(); i++)
        {
            ConFirm* confirm = new ConFirm;
            mObjectHelper.qvariant2qobject(confirmList[i].toMap(), confirm);
            mConfirmList.append(confirm);
        }
    }
    void setCompanyList(QVariantList companyList)
    {


        for (int i = 0; i < companyList.size(); i++)
        {
            expressCompany* company = new expressCompany;
            mObjectHelper.qvariant2qobject(companyList[i].toMap(), company);
            mCompanyList.append(company);
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
    QStringList mbigCellIdList ;
    QStringList mmiddleCellIdList ;
    QStringList msmallCellIdList;
    QStringList mvegetablesCellidList;
    QList<ConFirm*> mConfirmList;
    QList<expressCompany*> mCompanyList;
    QJson::QObjectHelper mObjectHelper;
    QStringList mblackNameList;
    QString mMobilePhone;
};

#endif // NOM1CARDLOGINRESP_H
