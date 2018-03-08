#ifndef EXCEPTIONCOMM_H
#define EXCEPTIONCOMM_H

#include "httpsync.h"
#include "respone.h"
#include "singleton.h"

struct cellErrorInfo{
    QString scabId;
    QString scellId;
    QString errType;         // 1 存件打不开 2 未关闭 3 取件打不开 4 快递员取件打不开
    QString relDeliveryId;   //取件打不开时,相应的派件ID
};

class CExceptioncomm
{
public:
    CExceptioncomm();

    void init();

    void reportCellErrors(QList<cellErrorInfo> infos,reportCellErrorsResp *respone);

    void repairCellErrors(cellErrorInfo infos,repairCellErrorsResp *respone);

    bool commReTrans(QString url, QString param);

    void reportCommonErrors(QString type,reportCommonErrorsResp *respone);

    void repairCommonErrors(QString type, QString time,repairCommonErrorsResp *respone);

    void getWeacherInfo(QString weatherCityNo,getWeacherInfoResp *respone);

    void saveTerminalIp(QString ipAddress,saveTerminalIpResp *respone);

    void isConfirmation(QString filename,QString creatTime,isConfirmationResp *respone);

    bool download(QString filename, QString fileMd5);

private:
    QString mUrl;
    QString md5Code;
    QString terminalId;

    DECLARE_SINGLETON_CLASS(CExceptioncomm)
};


typedef Singleton<CExceptioncomm> SCExceptioncomm;

#endif // EXCEPTIONCOMM_H
