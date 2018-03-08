#ifndef SERVERCOMM_H
#define SERVERCOMM_H

#include <QString>
#include <QStringList>
#include "singleton.h"
#include "httpsync.h"
#include "respone.h"


struct packInfo{
    QString packageId;
    QString mobilePhone;
    QString cellType;     // 1:表示大号箱  2：表示中号箱  3：表示小号箱
    QString cellId;
    QString isConfirm;
    QString isCheck;
};

/*
struct cellErrorInfo{
    QString scabId;
    QString scellId;
    QString errType;         // 1 打不开 2 未关闭
};


struct stockShowInfo{
    QString expressId;
    QString mobilePhone;
    QString boxId;
};
*/

class CServerComm
{
public:
    void init(QString ip, QString port);

    void m1CardLogin(QString m1CardId,m1CardLoginResp* respone);     //
    void genRanLoginCode(QString mobilePhone,getRanLoginCodeResp* respone);
    void noM1CardLogin(QString randomCode,m1CardLoginResp *respone);
    void getCellIdsByTerminalId(getCellIdsByTerminalIdResp *respone);
    void saveDeliverys(QString m1CardId,QList<packInfo> infos,saveDeliverysResp *respone );
    void genRanCodeAgain(QString mobilePhone,getRanCodeAgainResp *respone);
    void getDeliveryByCode(QString randomCode,getDeliveryByCodeResp *respone);
    void getExpressCompanys(QString m1CardId,getExpressCompanysResp *respone);
    void saveExpressEmp(QString optM1CardId,QString m1CardId ,QString companyId ,QString empName,\
                        QString cardId,QString mobilePhone,QString userAccount,QString userPwd,saveExpressEmpResp *respone);

    void getDeliveryConfirm(QString deliveryId,QString accountPaid,QString debtCost,QString payWay,QString cashPayMoney,QString mM1Card,QString forSelf,QString isExpired,getDeliveryConfirmResp *respone);

    void updatePackageStatus(QString deliveryId,QString type,updatePackageStatusResp *respone);

    void uploadDeliveryPhoto(QString url,QString imagefile,uploadDeliveryPhotoResp *respone);

    void uploadDebugFiles(QString fileName,uploadDeliveryPhotoResp *respone);

    void updateAccountPaid(QString deliveryId,QString debt,updateAccountPaidResp *respone);

    void getVIPCardInfo(QString deliveryId,QString m1Card,getVIPCardInfoResp *respone);

    void judgeStoredAccount(QString idcard,QString idcardpwd,judgeStoredAccountResp *respone);

    void rechargeToAccount(QString idcard,QString idcardpwd,rechargeToAccountResp *respone);

    void addVipCustomer(QString optM1CardId,QString m1Card,QString custName,QString mobilePhone,QString idCard,saveExpressEmpResp *respone);

    void getTariffInformation(getTariffInformationResp *respone);

    void checkBalance(QString account , checkBalanceResp *respone);

    void openScell(QString scellId,openScellResp *respone);

    void getSystemInfo(getSystemInfoResp *respone);

    void judgePwd(QString code, judgePwdResp *respone);

    void takeRecord(QString m1Card,QString coinCount,QString state);

    void financeGetPwd(QString phone,getRanCodeAgainResp*respone);

    QByteArray getDelsByVIPCard(QString m1Card, getDelsByVIPCardResp *respone);

    void VipBindingM1card(QString m1Card,QString mobilePhone,QString passwd,VipBindingM1cardResp *respone);

    //void saveTerminalIp(QString ipAddress,saveTerminalIpResp *respone);

    QByteArray deliveryTimeout(QString m1Card,QString mobilePhone,deliveryTimeoutResp *respone);

    void expTakeDelivery(QStringList deliveryId,expTakeDeliveryResp *respone);

    void checkOpenScell(QString m1Card ,checkOpenScellResp *respone);

    void noCardCheckOpenScell(QString passwd ,checkOpenScellResp *respone);

    QByteArray noCardDeliveryTimeout(QString passwd,deliveryTimeoutResp *respone);

    QByteArray expGetSaveDelivery(QString m1Card,QString passwd,expGetSaveDeliveryResp *respone);

    /*
    void reportCellErrors(QString terminalId,QList<cellErrorInfo> infos,reportCellErrorsResp *respone);

    void repairCellErrors(QString terminalId,QList<cellErrorInfo> infos,repairCellErrorsResp *respone);
*/
private:

    CServerComm();
    ~CServerComm();

    HttpSync* mHttpClient;
    QString   mUrl;
    QString   md5Code;
    QString   mTerminal;

    DECLARE_SINGLETON_CLASS(CServerComm)
};


typedef Singleton<CServerComm> SCServerComm;


#endif // SERVERCOMM_H
