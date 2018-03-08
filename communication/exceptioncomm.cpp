#include "exceptioncomm.h"
#include "httpparam.h"
#include "servercomm.h"
#include "qobjecthelper.h"
#include "parser.h"
#include "db.h"
#include <QDebug>
#include "shaEncy.h"
#include "httpupdate.h"
#include <QUuid>


using namespace QJson;

CExceptioncomm::CExceptioncomm()
{
   // mHttpClient = new HttpSync;

}

void CExceptioncomm::init()
{
    QString ip;
    QString port;
    SCDatBase::instance()->getServerInfo(&ip, &port);
    mUrl = "http://" +ip +":" + port;

    QString md5Id = SCDatBase::instance()->getMdCode();
    md5Code = QString(SHA1(md5Id.toStdString()).c_str());

    terminalId = SCDatBase::instance()->getTerminalId();
}


void CExceptioncomm::reportCellErrors(QList<cellErrorInfo> infos, reportCellErrorsResp *respone)
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

    paramMap.insert("terminalId", terminalId);

    for (int i = 0; i < infos.size(); i++)
    {
        QVariantMap  packeInfo;
        packeInfo.insert("scabId", infos[i].scabId);
        packeInfo.insert("scellId", infos[i].scellId);
        packeInfo.insert("errType",infos[i].errType);
        packeInfo.insert("relDeliveryId",infos[i].relDeliveryId);
        list << packeInfo;

        failCell = failCell + infos[i].scellId + "|";
    }

    paramMap.insert("cellErrorLogArr", list);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "reportCellErrors send:" << sparam;
    HttpSync client;
    //client.post()

    strRespone = client.post(mUrl + "/lxyz/soapManager_reportCellErrors.do",  sparam);

    qDebug() << "reportCellErrors recv:" << strRespone;
    //取件打不开的重发
    if (strRespone.indexOf("true") == -1 && infos[0].errType == "3")
    {
       // failCell = infos[0].scellId;
        errType = "3";
        SCDatBase::instance()->insertServerException(mUrl + "/lxyz/soapManager_reportCellErrors.do", sparam,failCell,errType);
    }

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);

}

void CExceptioncomm::repairCellErrors(cellErrorInfo infos, repairCellErrorsResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;
    QVariantList list;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("terminalId", terminalId);

    QVariantMap  packeInfo;
    packeInfo.insert("scabId", infos.scabId);
    packeInfo.insert("scellId", infos.scellId);
    packeInfo.insert("errType",infos.errType);
    list << packeInfo;


    paramMap.insert("cellErrorLogArr", list);

    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "repairCellErrors send:" << sparam;

    HttpSync client;
    strRespone = client.post(mUrl + "/lxyz/soapManager_repairCellErrors.do",  sparam);

    qDebug() << "repairCellErrors recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

bool CExceptioncomm::commReTrans(QString url, QString param)
{
    HttpSync client;
    Parser      parser;
    qDebug() << "--info--: commReTrans send:" << param ;
    QString respone = client.post(url, param);
    qDebug() << "--info--: commReTrans recv" << respone ;
    if(respone.isEmpty())
    {
        return false;
    }
    else
    {
        QVariantMap retMap = parser.parse(respone.toAscii()).toMap();      
        QVariant ret = retMap.value("errCode");
        if((ret == "0000") && url.contains("saveDeliverys"))
        {
            saveDeliverysResp res;

            QObjectHelper::qvariant2qobject(retMap, &res);
            for(int i = 0; i < res.getDeliveryInfo().size(); i++)
            {
                QString delivery = res.getDeliveryInfo().at(i)->deliveryId();
                QString opencell = res.getDeliveryInfo().at(i)->scellId();
                QString getcode  = res.getDeliveryInfo().at(i)->getCode();
                QString celltype = res.getDeliveryInfo().at(i)->scellType();
                SCDatBase::instance()->insertTakePackageInfo(delivery,opencell,getcode,celltype);

            }
        }
        return ( ret == CONNECT_ERROR || ret == DEVICE_ERROR) ? false : true;
    }
  //  return respone.indexOf("true") == -1 ? false : true;
}


void CExceptioncomm::reportCommonErrors(QString type,reportCommonErrorsResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;
    QVariantList list;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("terminalId", terminalId);


    QVariantMap  packeInfo;
    packeInfo.insert("errType",type);
    list << packeInfo;


    paramMap.insert("commonErrorLogArr", list);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "reportCommonErrors send: " << sparam;

    HttpSync client;
    strRespone = client.post(mUrl + "/lxyz/soapManager_reportCommonErrors.do",  sparam);

    qDebug() << "reportCommonErrors recv: " << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CExceptioncomm::repairCommonErrors(QString type,QString time, repairCommonErrorsResp *respone)
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

    paramMap.insert("terminalId", terminalId);

    QVariantMap  packeInfo;
    packeInfo.insert("errType",type);
    packeInfo.insert("errCreateTime",time);
    list << packeInfo;


    paramMap.insert("commonErrorLogArr", list);   
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "repairCommonErrors send: " << sparam;

    HttpSync client;
    strRespone = client.post(mUrl + "/lxyz/soapManager_repairCommonErrors.do",  sparam);

    qDebug() << "repairCommonErrors recv: " << strRespone;

    if (strRespone.indexOf("true") == -1)
    {
        errType = "4";
        SCDatBase::instance()->insertServerException(mUrl + "/lxyz/soapManager_repairCommonErrors.do", sparam,failCell,errType);
    }

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

void CExceptioncomm::getWeacherInfo(QString weatherCityNo, getWeacherInfoResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("terminalId",terminalId);
    paramMap.insert("weatherCityNo",weatherCityNo);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);


    QString sparam = serializer.serialize(paramMap);

    qDebug() << "getWeacherInfo send:" << sparam;

    HttpSync client;
    strRespone = client.post(mUrl + "/lxyz/soapManager_getWeacherInfo.do",  sparam);

    qDebug() << "getWeacherInfo recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

//定时回传IP地址接口
void CExceptioncomm::saveTerminalIp(QString ipAddress, saveTerminalIpResp *respone)
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
    terminalIp.insert("terminalId", terminalId);
    terminalIp.insert("ipAddress",ipAddress);

    paramMap.insert("terminalIp",terminalIp);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("randomUUID",strUuId);

    QString sparam = serializer.serialize(paramMap);

    qDebug() << "saveTerminalIp send:" << sparam;

    HttpSync client;
    strRespone = client.post(mUrl + "/lxyz/soapManager_saveTerminalIp.do?", sparam);

    qDebug() << "saveTerminalIp recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}



void CExceptioncomm::isConfirmation(QString filename, QString creatTime, isConfirmationResp *respone)
{
    QVariantMap paramMap;
    HttpParam   serializer;
    Parser      parser;
    QString     strRespone;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    paramMap.insert("terminalId",terminalId);
    paramMap.insert("fileFileName",filename);
    paramMap.insert("encryptCode",md5Code);
    paramMap.insert("creatTime",creatTime);
    paramMap.insert("randomUUID",strUuId);


    QString sparam = serializer.serialize(paramMap);

    qDebug() << "isConfirmation send:" << sparam;

    HttpSync client;
    strRespone = client.post(mUrl + "/lxyz/soapManager_isConfirmation.do",  sparam);

    qDebug() << "isConfirmation recv:" << strRespone;

    QObjectHelper::qvariant2qobject(parser.parse(strRespone.toAscii()).toMap(), respone);
}

bool CExceptioncomm::download(QString filename, QString fileMd5)
{
    bool ret = false;
    httpUpdate https;
    QString url = mUrl + "/lxyz/soapManager_download.do?";

    qDebug() << "download send fileMd5:" << fileMd5;

    ret = https.getAppInfo(url,filename,terminalId,md5Code,fileMd5);

    qDebug() << "download recv:" << ret;

    return ret;
}


