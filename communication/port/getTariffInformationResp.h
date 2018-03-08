#ifndef GETTARIFFINFORMATIONRESP_H
#define GETTARIFFINFORMATIONRESP_H


#include <QObject>
#include <QString>


class getTariffInformationResp : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString success              READ success                 WRITE setSuccess)
    Q_PROPERTY(QString errCode              READ errCode                 WRITE setErrcode)
    Q_PROPERTY(QString errMessage           READ errMessage              WRITE setErrMessage)
    Q_PROPERTY(QString tariffTypeStr        READ tariffTypeStr           WRITE setTariffTypeStr)
    Q_PROPERTY(QString bigCellTariffStr     READ bigCellTariffStr        WRITE setBigCellTariffStr)
    Q_PROPERTY(QString centreCellTariffStr  READ centreCellTariffStr     WRITE setCentreCellTariffStr)
    Q_PROPERTY(QString smallCellTariffStr   READ smallCellTariffStr      WRITE setSmallCellTariffStr)
    Q_PROPERTY(QString vegetablesTariffStr  READ vegetablesTariffStr     WRITE setVegetablesTariffStr)
    Q_PROPERTY(QString vipPercent           READ vipPercent              WRITE setVipPercent)
    Q_PROPERTY(QString customerPrice        READ customerPrice           WRITE setCustomerPrice)
    Q_PROPERTY(QString defaultDebtdays      READ defaultDebtdays         WRITE setDefaultDebtdays)
    Q_PROPERTY(QString defaultDebtmoney     READ defaultDebtmoney        WRITE setDefaultDebtmoney)
    Q_PROPERTY(QString defaultDebSwitch     READ defaultDebSwitch        WRITE setDefaultDebSwitch)
    Q_PROPERTY(QString expireStep           READ expireStep              WRITE setExpireStep)
    Q_PROPERTY(QString stepMoney            READ stepMoney               WRITE setStepMoney)
    Q_PROPERTY(QString appPercent           READ appPercent              WRITE setAppPercent)
    Q_PROPERTY(QString weightPercent        READ weightPercent           WRITE setWeightPercent)

public:

    getTariffInformationResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString errMessage() const{return mErrMessage;}
    QString tariffTypeStr() const{return mTariffTypeStr;}
    QString bigCellTariffStr() const{return mBigCellTariffStr;}
    QString centreCellTariffStr() const{return mCentreCellTariffStr;}
    QString smallCellTariffStr() const{return mSmallCellTariffStr;}
    QString vegetablesTariffStr() const{return mVegetablesTariffStr;}
    QString vipPercent() const{return mVipPercent;}
    QString customerPrice() const{return mCustomerPrice;}
    QString defaultDebtdays() const{return mDefaultDebtdays;}
    QString defaultDebtmoney() const{return mDefaultDebtmoney;}
    QString defaultDebSwitch() const{return mDefaultDebSwitch;}
    QString expireStep() const{return mExpireStep;}
    QString stepMoney() const{return mStepMoney;}
    QString appPercent() const{return mAppPercent;}
    QString weightPercent() const{return mWeightPercent;}

    ~getTariffInformationResp()
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
        mErrMessage = errMessage;
    }

    void setTariffTypeStr(QString tariffTypeStr)
    {
        mTariffTypeStr = tariffTypeStr;
    }

    void setBigCellTariffStr(QString bigCellTariffStr)
    {
        mBigCellTariffStr = bigCellTariffStr;
    }

    void setCentreCellTariffStr(QString centreCellTariffStr)
    {
        mCentreCellTariffStr = centreCellTariffStr;
    }

    void setSmallCellTariffStr(QString smallCellTariffStr)
    {
        mSmallCellTariffStr = smallCellTariffStr;
    }

    void setVegetablesTariffStr(QString vegetablesTariffStr)
    {
        mVegetablesTariffStr = vegetablesTariffStr;
    }

    void setVipPercent(QString vipPercent)
    {
        mVipPercent = vipPercent;
    }

    void setCustomerPrice(QString customerPrice)
    {
        mCustomerPrice = customerPrice;
    }
    void setDefaultDebtdays(QString defaultDebtdays)
    {
        mDefaultDebtdays = defaultDebtdays;
    }
    void setDefaultDebtmoney(QString defaultDebtmoney)
    {
        mDefaultDebtmoney = defaultDebtmoney;
    }
    void setDefaultDebSwitch(QString defaultDebSwitch)
    {
        mDefaultDebSwitch = defaultDebSwitch;
    }

    void setExpireStep(QString expireStep)
    {
        mExpireStep = expireStep;
    }
    void setStepMoney(QString stepMoney)
    {
        mStepMoney = stepMoney;
    }

    void setAppPercent(QString appPercent)
    {
        mAppPercent = appPercent;
    }

    void setWeightPercent(QString weightPercent)
    {
        mWeightPercent = weightPercent;
    }

private:
    QString mSuccess ;  //
    QString mErrcode;
    QString mErrMessage;
    QString mTariffTypeStr;
    QString mBigCellTariffStr;
    QString mCentreCellTariffStr;
    QString mSmallCellTariffStr;
    QString mVegetablesTariffStr;
    QString mVipPercent;
    QString mCustomerPrice;
    QString mDefaultDebtdays;
    QString mDefaultDebtmoney;
    QString mDefaultDebSwitch;
    QString mExpireStep;
    QString mStepMoney;
    QString mAppPercent;
    QString mWeightPercent;
};
#endif // GETTARIFFINFORMATIONRESP_H
