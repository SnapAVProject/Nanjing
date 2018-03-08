
/*
 *  table  : config  
 *          key_id  termialId   ip  port  bg_image_path
 *            01       
 *  
 */

#ifndef _DB_H_
#define _DB_H_


#include <QString>
#include "KompexSQLiteDatabase.h"
#include "KompexSQLiteStatement.h"
#include "singleton.h"
#include <QMap>

using namespace  Kompex;


typedef struct{
    qint8 index;
    qint8 cabinetId;
    qint8 cellId;
    qint8 cellType;    // 1.大 2.中 3.小
    QString errType;
}CellDesc_t;

typedef struct{
    qint8 index;
    QString url;
    QString param;
    QString cellId;
    QString errType;
}ServerDesc_t;


typedef struct{
    qint8 index;
    QString deliveryId;
    qint8 debt;
}DebtDesc_t;

typedef struct{
    qint8 bigCell;
    qint8 middleCell;
    qint8 smallCell;
    qint8 vegetableCell;
}CellInfoDesc_t;


typedef struct{
    qint8 index;
    QString delivery;
    QString opencell;
    QString getcode;
    QString celltype;
}TakeInfoDesc_t;

class CDatBase{

public:

    bool init(QString dbPath);
	
    //编辑配置信息
    bool setTerminalId(QString terminalId);	
	QString getTerminalId();	
    bool setServerInfo(QString ip, QString port);	
    bool getServerInfo(QString *ip, QString *port);
    bool setUpsInfo(QString upsInfo);
    bool getUpsInfo(QString *upsInfo);
    bool setMdCode(QString mdId);
    QString getMdCode();
    bool setCityId(QString cityId);
    QString getCityId();
    bool setUploadImages(QString loadInfo);
    QString getUploadImages();

    bool setGoodsCheck(QString goodsCheck);
    bool getGoodsCheck();
    bool setAppVersion(QString appInfo);
    QString getAppVersion();


    bool setBackgoundImgPath(QString path);
	QString getBackgoundImgPath();


    //可用箱子数量
    bool setBoxCount(qint8 big,qint8 middle,qint8 small,qint8 vegetable);
    bool getBoxCount(CellInfoDesc_t &cellInfo);

    //本地存储取件码信息数据库
    int  insertTakePackageInfo(QString delivery, QString opencell,QString getcode ,QString celltype);
    bool queryTakePackageInfo(TakeInfoDesc_t &takeInfo,QString getcode);
    bool deleteTakePackageInfo(QString deliveryId);
    bool deleteTakePackageCellInfo(QString opencell);
    void deleteTable();


    //未关闭箱门\打不开箱门数据库
    int  insertOpenException(qint8 cabinetId, qint8 cellId,qint8 cellType,QString errType);
    void queryOpenException(QList<CellDesc_t> & cells);
    bool deleteOpenException(qint8 index);
    bool setOpenException(qint8 index);

    //网络通信错误的信息
    int  insertServerException(QString url,QString param,QString box,QString errType);
    void queryServerException(QList<ServerDesc_t> & serverWaitInfo);
    bool deleteServerException(qint8 index);

    //欠费信息
    int insertDebtException(QString deliveryId , qint8 debt);
    void queryDebtException(QList<DebtDesc_t> & cells);
    void setDebtException(qint8 index,qint8 debt);
    bool deleteDebtException(qint8 index);




private:
    CDatBase() {}
    ~CDatBase() {}

    QString mDbPath;

    SQLiteDatabase *sqliteDb;

    SQLiteStatement *sqlStatement;

    DECLARE_SINGLETON_CLASS(CDatBase)

};

typedef Singleton<CDatBase> SCDatBase;


#endif
