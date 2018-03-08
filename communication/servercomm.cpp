#include <QDebug>
#include "httpparam.h"
#include "servercomm.h"
#include "qobjecthelper.h"
#include "parser.h"
#include "db.h"
#include "configure/db.h"
#include "urlencode.h"
#include "fileencode.h"
#include "shaEncy.h"
#include <QUuid>

using namespace QJson;

CServerComm::CServerComm()
{
    mHttpClient = new HttpSync;
}

CServerComm::~CServerComm()
{
    delete mHttpClient;
}

void CServerComm::init(QString ip, QString port)
{
    mUrl = "http://" +ip +":" + port;
    QString md5Id = SCDatBase::instance()->getMdCode();
    md5Code = QString(SHA1(md5Id.toStdString()).c_str());
    mTerminal = SCDatBase::instance()->getTerminalId();

}

void CServerComm::m1CardLogin(QString m1CardId,m1CardLoginResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("m1CardId", m1CardId);
    paramMap.insert("terminalId", mTerminal);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "m1CardLogin send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_m1CardLogin.do",  sparam);

    qDebug() << "info -- m1CardLogin, recv : " << strRespone;
    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);

}

void CServerComm::genRanLoginCode(QString mobilePhone,getRanLoginCodeResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("mobilePhone", mobilePhone);
    paramMap.insert("terminalId", mTerminal);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "genRanLoginCode send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_genRanLoginCode.do", sparam);

    qDebug() << "genRanLoginCode recv: " << strRespone;
    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CServerComm::noM1CardLogin(QString randomCode, m1CardLoginResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("randomCode", randomCode);
    paramMap.insert("terminalId", mTerminal);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "noM1CardLogin send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_noM1CardLogin.do",  sparam);

    qDebug() << "noM1CardLogin recv: " << strRespone;
    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CServerComm::getCellIdsByTerminalId(getCellIdsByTerminalIdResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();


    paramMap.insert("terminalId", mTerminal);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "getCellIdsByTerminalId send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_getCellIdsByTerminalId.do",  sparam);

    qDebug() << " getCellIdsByTerminalId recv: " << strRespone;
    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}



void CServerComm::saveDeliverys(QString m1CardId, QList<packInfo> infos , saveDeliverysResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;
    QVariantList list;
    QString failCell;
    QString errType;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("m1CardId", m1CardId);
    paramMap.insert("terminalId", mTerminal);

    for (int i = 0; i < infos.size(); i++)
    {
        QVariantMap  packeInfo;
        packeInfo.insert("packageId", infos[i].packageId);
        packeInfo.insert("mobilePhone", infos[i].mobilePhone);
        packeInfo.insert("scellType",infos[i].cellType);
        packeInfo.insert("scabId",   infos[i].cellId.mid(0,infos[i].cellId.size() - 2) );
        packeInfo.insert("scellId", infos[i].cellId );
        packeInfo.insert("isConfirm", infos[i].isConfirm);
        packeInfo.insert("goodsCheckingStatus",infos[i].isCheck);
        list << packeInfo;

        failCell =  failCell + infos[i].cellType + "," + infos[i].cellId + "|";
    }
    //qDebug() << "m1CardId :" << m1CardId << "terninalId: "  << terminalId;

    errType = "1";

    paramMap.insert("deliveryArr", list);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "saveDeliverys send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_saveDeliverys.do",  sparam);

    qDebug() << "saveDeliverys recv:" << strRespone;

    if (strRespone.indexOf("true") == -1)
    {
        sparam = sparam + "&isRepeat=1";  //重发
        SCDatBase::instance()->insertServerException(mUrl + "/lxyz/soapManager_saveDeliverys.do", sparam,failCell,errType);
    }

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);

//    if(strRespone == "" || respone->errCode() == CONNECT_ERROR)
//    {
//        SCDatBase::instance()->insertServerException(mUrl + "/lxyz/soapManager_saveDeliverys.do", sparam,failCell,errType);
//    }
}

void CServerComm::genRanCodeAgain(QString mobilePhone, getRanCodeAgainResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();


    paramMap.insert("mobilePhone", mobilePhone);
    paramMap.insert("terminalId", mTerminal);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);
    qDebug() << "genRanCodeAgain send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_genRanCodeAgain.do",  sparam);

    qDebug() << "genRanCodeAgain recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CServerComm::getDeliveryByCode(QString randomCode, getDeliveryByCodeResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("randomCode", randomCode);
    paramMap.insert("terminalId", mTerminal);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);
    qDebug() << "getDeliveryByCode send" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_getDeliveryByCode.do",  sparam);

    qDebug() << "getDeliveryByCode recv" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CServerComm::getExpressCompanys(QString m1CardId,getExpressCompanysResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("m1CardId", m1CardId);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);


    QString sparam = serializer.serialize(paramMap);
    qDebug() << "getExpressCompanys send" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_getExpressCompanys.do",  sparam);
    qDebug() << "getExpressCompanys recv" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CServerComm::saveExpressEmp(QString optM1CardId, QString m1CardId, QString companyId, QString empName, \
                 QString cardId, QString mobilePhone, QString userAccount, QString userPwd, saveExpressEmpResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    QVariantMap  packeInfo;
    packeInfo.insert("optM1CardId", optM1CardId);
    packeInfo.insert("m1CardId", m1CardId);
    packeInfo.insert("companyId", companyId);
    packeInfo.insert("empName", empName);
    packeInfo.insert("cardId", cardId);
    packeInfo.insert("mobilePhone", mobilePhone);
    packeInfo.insert("userAccount", userAccount);
    packeInfo.insert("userPwd", userPwd);

    paramMap.insert("terminalId",mTerminal);
    paramMap.insert("soapEmp",packeInfo);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);
    qDebug() << "saveExpressEmp send" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_saveExpressEmp.do",  sparam);

    qDebug() << "saveExpressEmp recv" << strRespone;
    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CServerComm::getDeliveryConfirm(QString deliveryId, QString accountPaid, QString debtCost, QString payWay,QString cashPayMoney, QString mM1Card,QString forSelf,QString isExpired,getDeliveryConfirmResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;
    QString failCell = deliveryId;
    QString errType = "2";

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();


    paramMap.insert("deliveryId", deliveryId);
    paramMap.insert("terminalId", mTerminal);
    paramMap.insert("accountPaid",accountPaid);
    paramMap.insert("debtCost",debtCost);
    paramMap.insert("payWay",payWay);
    paramMap.insert("cashPayMoney",cashPayMoney);
    paramMap.insert("m1CardId",mM1Card);
    paramMap.insert("payForSelf",forSelf);
    paramMap.insert("isExpired",isExpired);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "getDeliveryConfirm send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_getDeliveryConfirm.do",  sparam);

    qDebug() << "getDeliveryConfirm recv:" << strRespone;

    if (strRespone.indexOf("true") == -1)
    {
        SCDatBase::instance()->insertServerException(mUrl + "/lxyz/soapManager_getDeliveryConfirm.do", sparam,failCell,errType);
    }

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CServerComm::updatePackageStatus(QString deliveryId,QString type,updatePackageStatusResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("deliveryId", deliveryId);
    paramMap.insert("terminalId", mTerminal);
    paramMap.insert("confirmType", type);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);
    qDebug() << "updatePackageStatus send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_updatePackageStatus.do", sparam);
    qDebug() << "updatePackageStatus recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CServerComm::uploadDeliveryPhoto(QString url, QString imagefile, uploadDeliveryPhotoResp *respone)
{

    Parser      parser;
    QString     strRespone;
    Http mhttp;

    strRespone = mhttp.postFile(url,imagefile,md5Code);

    qDebug() << "uploadDeliveryPhoto recv : " << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CServerComm::uploadDebugFiles(QString fileName,uploadDeliveryPhotoResp *respone)
{
    Parser      parser;
    QString  strRespone;
    FileHttp fileHttp;

    QString url = mUrl + "/lxyz/soapManager_uploadDebugFiles.do";

    strRespone = fileHttp.postFile(url,fileName,md5Code,mTerminal);

    qDebug() << "uploadDebugFiles recv : " << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);

}


void CServerComm::updateAccountPaid(QString deliveryId, QString debt, updateAccountPaidResp *respone)
{

    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;


    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("terminalId", mTerminal);
    paramMap.insert("deliveryId", deliveryId);
    paramMap.insert("accountPaid", debt);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "updateAccountPaid send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_updateAccountPaid.do",  sparam);

    qDebug() << "updateAccountPaid recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}


void CServerComm::getVIPCardInfo(QString deliveryId,QString m1Card, getVIPCardInfoResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;


    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("m1CardId", m1Card);
    paramMap.insert("deliveryId", deliveryId);
    paramMap.insert("terminalId", mTerminal);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "getVIPCardInfo send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_getVIPCardInfo.do",  sparam);

    qDebug() << "getVIPCardInfo recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}


void CServerComm::judgeStoredAccount(QString idcard, QString idcardpwd, judgeStoredAccountResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    QVariantMap priceInfo;

    priceInfo.insert("m1Card", idcard);
    priceInfo.insert("cardPwd",idcardpwd);

    paramMap.insert("terminalId",mTerminal);
    paramMap.insert("storedAccount",priceInfo);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "judgeStoredAccount send:" << sparam;
    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_judgeStoredAccount.do",  sparam);

    qDebug() << "judgeStoredAccount recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CServerComm::rechargeToAccount(QString idcard, QString idcardpwd, rechargeToAccountResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    QVariantMap priceInfo;
    priceInfo.insert("m1Card", idcard);
    priceInfo.insert("cardPwd",idcardpwd);
    paramMap.insert("storedAccount",priceInfo);
    paramMap.insert("terminalId",mTerminal);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "rechargeToAccount send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_rechargeToAccount.do",  sparam);

    qDebug() << "rechargeToAccount recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}


void CServerComm::addVipCustomer(QString optM1CardId,QString m1Card, QString custName, QString mobilePhone, QString idCard, saveExpressEmpResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    QVariantMap memberInfo;
    memberInfo.insert("m1Card", m1Card);
    memberInfo.insert("custName",custName);
    memberInfo.insert("mobilePhone", mobilePhone);
    memberInfo.insert("idCard",idCard);
    memberInfo.insert("optM1CardId",optM1CardId);

    paramMap.insert("terminalId",mTerminal);
    paramMap.insert("customer",memberInfo);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "addVipCustomer send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_addVipCustomer.do",  sparam);

    qDebug() << "addVipCustomer recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}


void CServerComm::getTariffInformation(getTariffInformationResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("terminalId", mTerminal);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "getTariffInformation send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_getTariffInformation.do",  sparam);

    qDebug() << "getTariffInformation recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}


void CServerComm::checkBalance(QString account, checkBalanceResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    QVariantMap memberInfo;
    memberInfo.insert("account", account);

    paramMap.insert("terminalId",mTerminal);
    paramMap.insert("account",memberInfo);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "checkBalance send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_checkBalance.do",  sparam);

    qDebug() << "checkBalance recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}


void CServerComm::openScell(QString scellId, openScellResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    QVariantMap memberInfo;
    memberInfo.insert("scellId", scellId);
    paramMap.insert("cell",memberInfo);
    paramMap.insert("terminalId",mTerminal);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "openScell send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_openScell.do",  sparam);

    qDebug() << "openScell recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CServerComm::getSystemInfo(getSystemInfoResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("terminalId",mTerminal);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "getSystemInfo send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_getSystemInfo.do",  sparam);

    qDebug() << "getSystemInfo recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}


void CServerComm::judgePwd(QString code, judgePwdResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("terminalId",mTerminal);
    paramMap.insert("randomCode",code);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "judgePwd send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_judgePwd.do",  sparam);

    qDebug() << "judgePwd recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CServerComm::takeRecord(QString m1Card, QString coinCount, QString state)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    QString     strRespone;
    QString     failCell = "0820";
    QString     errType;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("m1CardId", m1Card);
    paramMap.insert("terminalId",mTerminal);
    paramMap.insert("tookMoney",coinCount);
    paramMap.insert("state",state);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "takeRecord send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_takeRecord.do",  sparam);

    qDebug() << "takeRecord recv:" << strRespone;

    if (strRespone.indexOf("true") == -1)
    {
        errType = "0";
        SCDatBase::instance()->insertServerException(mUrl + "/lxyz/soapManager_takeRecord.do", sparam,failCell,errType);
    }

 //   QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);

}

void CServerComm::financeGetPwd(QString phone, getRanCodeAgainResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("terminalId",mTerminal);
    paramMap.insert("mobilePhone",phone);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "financeGetPwd send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_financeGetPwd.do",  sparam);

    qDebug() << "financeGetPwd recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}


QByteArray CServerComm::getDelsByVIPCard(QString m1Card, getDelsByVIPCardResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("terminalId",mTerminal);
    paramMap.insert("m1CardId",m1Card);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "getDelsByVIPCard send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_getDelsByVIPCard.do",  sparam);

    qDebug() << "getDelsByVIPCard recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);

    return strRespone.toAscii();
}

void CServerComm::VipBindingM1card(QString m1Card, QString mobilePhone, QString passwd, VipBindingM1cardResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    QVariantMap memberInfo;
    memberInfo.insert("mobilePhone", mobilePhone);
    memberInfo.insert("cardPwd",passwd);
    paramMap.insert("buyRecord",memberInfo);
    paramMap.insert("m1CardId",m1Card);
    paramMap.insert("terminalId",mTerminal);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "VipBindingM1card send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_VipBindingM1card.do",  sparam);

    qDebug() << "VipBindingM1card recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

/*
//定时回传IP地址接口
void CServerComm::saveTerminalIp(QString ipAddress, saveTerminalIpResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    QVariantMap terminalIp;
    terminalIp.insert("terminalId", mTerminal);
    terminalIp.insert("ipAddress",ipAddress);

    paramMap.insert("terminalIp",terminalIp);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "saveTerminalIp send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/soapManager_saveTerminalIp.do?", sparam);

    qDebug() << "saveTerminalIp recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}
*/


QByteArray CServerComm::deliveryTimeout(QString m1Card, QString mobilePhone,deliveryTimeoutResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QVariantMap deliveryInfo;
    deliveryInfo.insert("terminalId", mTerminal);
    deliveryInfo.insert("m1CardId",m1Card);
    deliveryInfo.insert("mobilePhone",mobilePhone);
    deliveryInfo.insert("encryptCode",md5Code);

    paramMap.insert("deliveryTimeoutReq",deliveryInfo);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "deliveryTimeout send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/qtSoapManage_deliveryTimeout.do?", sparam);

    qDebug() << "deliveryTimeout recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);

    return strRespone.toAscii();
}


void CServerComm::expTakeDelivery(QStringList deliveryId, expTakeDeliveryResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;
    QString     failCell;
    QString     errType;
    QVariantList list;



    for (int i = 0; i < deliveryId.size(); i++)
    {
        QString strDelivery;
        failCell = failCell + deliveryId.at(i) + "|";
        strDelivery = deliveryId.at(i);
        list << strDelivery;
    }

    QVariantMap deliveryInfo;
    deliveryInfo.insert("deliveryIdList",list);
    deliveryInfo.insert("terminalId", mTerminal);
    deliveryInfo.insert("encryptCode",md5Code);

    paramMap.insert("expTakeDeliveryReq",deliveryInfo);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "expTakeDelivery send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/qtSoapManage_expTakeDelivery.do?", sparam);

    qDebug() << "expTakeDelivery recv:" << strRespone;

    QVariantMap retMap = parser.parse(strRespone.toAscii()).toMap();
    QVariant ret = retMap.value("errCode");
    if (strRespone == "" || ret == CONNECT_ERROR)
    {
        errType = "5";
        SCDatBase::instance()->insertServerException(mUrl + "/lxyz/qtSoapManage_expTakeDelivery.do", sparam,failCell,errType);
    }

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);

}




void CServerComm::checkOpenScell(QString m1Card, checkOpenScellResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QVariantMap cardInfo;
    cardInfo.insert("terminalId", mTerminal);
    cardInfo.insert("m1CardId",m1Card);
    cardInfo.insert("encryptCode",md5Code);

    paramMap.insert("checkOpenScellReq",cardInfo);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "checkOpenScell send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/qtSoapManage_checkOpenScell.do?", sparam);

    qDebug() << "checkOpenScell recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);

}


void CServerComm::noCardCheckOpenScell(QString passwd, checkOpenScellResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QVariantMap cardInfo;
    cardInfo.insert("terminalId", mTerminal);
    cardInfo.insert("passwd",passwd);
    cardInfo.insert("encryptCode",md5Code);

    paramMap.insert("noCardCheckOpenScellReq",cardInfo);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "noCardCheckOpenScell send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/qtSoapManage_noCardCheckOpenScell.do?", sparam);

    qDebug() << "noCardCheckOpenScell recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);

}


QByteArray CServerComm::noCardDeliveryTimeout(QString passwd,deliveryTimeoutResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QVariantMap deliveryInfo;
    deliveryInfo.insert("terminalId", mTerminal);
    deliveryInfo.insert("passwd",passwd);
    deliveryInfo.insert("encryptCode",md5Code);

    paramMap.insert("noCardDeliveryTimeoutReq",deliveryInfo);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "noCardDeliveryTimeout send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/qtSoapManage_noCardDeliveryTimeout.do?", sparam);

    qDebug() << "noCardDeliveryTimeout recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);

    return strRespone.toAscii();
}


QByteArray CServerComm::expGetSaveDelivery(QString m1Card, QString passwd, expGetSaveDeliveryResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QVariantMap deliveryInfo;
    deliveryInfo.insert("terminalId", mTerminal);
    deliveryInfo.insert("m1Card",m1Card);
    deliveryInfo.insert("passwd",passwd);
    deliveryInfo.insert("encryptCode",md5Code);

    paramMap.insert("expGetSaveDeliveryReq",deliveryInfo);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "expGetSaveDelivery send:" << sparam;

    strRespone = mHttpClient->post(mUrl + "/lxyz/qtSoapManage_expGetSaveDelivery.do?", sparam);

    qDebug() << "expGetSaveDelivery recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);

    return strRespone.toAscii();
}
