#include "db.h"
#include "KompexSQLiteException.h"
#include "KompexSQLitePrerequisites.h"
#include <QDebug>
#include <QString>
#include <string>
#include <sstream>

using namespace std;


bool CDatBase::init(QString dbpath)
{

    mDbPath = dbpath;
    int configCount = 0;

    bool ret = true;

    try{
        sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), \
                                                      SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0);

        sqlStatement = new SQLiteStatement(sqliteDb);

        sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("CREATE TABLE IF NOT EXISTS [tb_config] (\
                      [id]             TEXT(4) NOT NULL, \
                      [terminal_id]    TEXT(16), \
                      [server_ip]      TEXT(32), \
                      [server_port]    TEXT(5),\
                      [background_img] TEXT(32),\
                      [ups_status]     TEXT(5),\
                      [md5_id]         TEXT(128),\
                      [city_id]        TEXT(15),\
                      [upload_image]   TEXT(5),\
                      [goods_check]    TEXT(5),\
                      [version_app]    TEXT(30));");   //ups_status:flase:断电   true:正常 upload_image:flase:关闭   true:开启
        sqlStatement->ExecuteAndFree();

//        sqlStatement->Sql("alter table tb_config modify terminal_id varchar(32)");
//        sqlStatement->ExecuteAndFree();

        //--------------------------------------------------------------------
        //箱门打不开异常
        //  : errType 1. 存件箱门未关闭     2. 取件箱门未关闭
        //--------------------------------------------------------------------

        sqlStatement->Sql("CREATE TABLE IF NOT EXISTS [tb_open_exception] (\
                      [id]         TEXT(3) NOT NULL, \
                      [is_used]    TEXT(5) NOT NULL, \
                      [cabinet_id] TEXT(2) , \
                      [cell_id]    TEXT(2) , \
                      [cell_type]  TEXT(2),\
                      [errType]    TEXT(3));");
        sqlStatement->ExecuteAndFree();
        sqlStatement->Sql("SELECT COUNT(*) FROM tb_open_exception" );
        while (sqlStatement->FetchRow())
        {
            configCount = QString::fromAscii(sqlStatement->GetColumnString(0).c_str()).toInt();
        }
        sqlStatement->FreeQuery();

        if (!configCount)
        {
            for (int i = 0; i < 200; i++)
            {
                sqlStatement->Sql("INSERT   INTO tb_open_exception(id, is_used) VALUES ( @id, 'false');");
                sqlStatement->BindString(1,QString::number(i).toStdString());
                sqlStatement->ExecuteAndFree();
                //qDebug() << "insert tb_open_exception ...";
            }
        }

        //--------------------------------------------------------------------
        //本地保存存件信息
        //  :  is_used : 是否可用  delivery_id : 派件的ID  getcode_id : 取件码 opencell_id : 打开的箱门  cell_type : 箱子类型
        //--------------------------------------------------------------------

        sqlStatement->Sql("CREATE TABLE IF NOT EXISTS [tb_local_save] (\
                      [id]             TEXT(3) NOT NULL, \
                      [is_used]        TEXT(5) NOT NULL, \
                      [delivery_id]    TEXT(8),\
                      [getcode_id]     TEXT(8),\
                      [opencell_id]    TEXT(20) , \
                      [cell_type]      TEXT(2));");
        sqlStatement->ExecuteAndFree();
        sqlStatement->Sql("SELECT COUNT(*) FROM tb_local_save" );
        while (sqlStatement->FetchRow())
        {
            configCount = QString::fromAscii(sqlStatement->GetColumnString(0).c_str()).toInt();
        }
        sqlStatement->FreeQuery();

        if (!configCount)
        {
            for (int i = 0; i < 200; i++)
            {
                sqlStatement->Sql("INSERT   INTO tb_local_save(id, is_used) VALUES ( @id, 'false');");
                sqlStatement->BindString(1,QString::number(i).toStdString());
                sqlStatement->ExecuteAndFree();
                //qDebug() << "insert tb_open_exception ...";
            }
        }




        //-------------------------------------------------------------------
        //箱门数量信息
        //
        //
        //--------------------------------------------------------------------
        sqlStatement->Sql("CREATE TABLE IF NOT EXISTS [tb_cell_info] (\
                      [id]            TEXT(3) NOT NULL, \
                      [is_big]        TEXT(3) NOT NULL, \
                      [is_middle]     TEXT(3) , \
                      [is_small]      TEXT(3),\
                      [is_vegetable]  TEXT(3));");
        sqlStatement->ExecuteAndFree();

        sqlStatement->Sql("SELECT COUNT(*) FROM tb_cell_info" );
        while (sqlStatement->FetchRow())
        {
            configCount = QString::fromAscii(sqlStatement->GetColumnString(0).c_str()).toInt();
        }
        sqlStatement->FreeQuery();

        if (!configCount)
        {

            sqlStatement->Sql("INSERT INTO tb_cell_info(id, is_big,is_middle,is_small,is_vegetable) VALUES (1,8,24,48,10);");
            sqlStatement->ExecuteAndFree();

        }


        //-------------------------------------------------------------------
        //欠费信息
        //  ：debtValue 已经投的币值
        //
        //--------------------------------------------------------------------
        sqlStatement->Sql("CREATE TABLE IF NOT EXISTS [tb_debt_exception] (\
                      [id]         TEXT(3) NOT NULL, \
                      [is_used]    TEXT(5) NOT NULL, \
                      [deliveryId] TEXT(8) , \
                      [debtValue]  TEXT(2));");
        sqlStatement->ExecuteAndFree();
        sqlStatement->Sql("SELECT COUNT(*) FROM tb_debt_exception" );
        while (sqlStatement->FetchRow())
        {
            configCount = QString::fromAscii(sqlStatement->GetColumnString(0).c_str()).toInt();
        }
        sqlStatement->FreeQuery();

        if (!configCount)
        {
            for (int i = 0; i < 200; i++)
            {
                sqlStatement->Sql("INSERT   INTO tb_debt_exception(id, is_used) VALUES ( @id, 'false');");
                sqlStatement->BindString(1,QString::number(i).toStdString());
                sqlStatement->ExecuteAndFree();
                //qDebug() << "insert tb_open_exception ...";
            }
        }


        //--------------------------------------------------------------------
        //                           数据重复机制
        //               interface_name:  服务器地址
        //               param         ： 拚一个链接
        //               exit_door     ： 要发的箱门 （存件）
        //               errType       ： 重发的类型 0.取钱罐信息重发 1.存件重发  2.用户取件成功重发 3.取件箱门打不开上报重发 4.断电修复重发 5.快递员滞留件取出重发
        //--------------------------------------------------------------------

        sqlStatement->Sql("CREATE TABLE IF NOT EXISTS [tb_server_exception] (\
                      [id]             TEXT(3) NOT NULL, \
                      [is_used]        TEXT(5) NOT NULL, \
                      [interface_name] TEXT(256), \
                      [param]          TEXT(65535),\
                      [exit_door]      TEXT(65535),\
                      [errType]        TEXT(3) ) ;");
        sqlStatement->ExecuteAndFree();

        sqlStatement->Sql("SELECT COUNT(*) FROM tb_server_exception" );
        while (sqlStatement->FetchRow())
        {
            configCount = QString::fromAscii(sqlStatement->GetColumnString(0).c_str()).toInt();
        }
        sqlStatement->FreeQuery();

        if (!configCount)
        {
            for (int i = 0; i < 200; i++)
            {
                sqlStatement->Sql("INSERT   INTO tb_server_exception(id, is_used) VALUES ( @id, 'false');");
                sqlStatement->BindString(1,QString::number(i).toStdString());
                sqlStatement->ExecuteAndFree();
                //qDebug() << "insert tb_open_exception ...";
            }
        }

        //--------------------------------------------------------------------


        sqlStatement->Sql("SELECT COUNT(*) FROM tb_config" );
        while (sqlStatement->FetchRow())
        {
            configCount = QString::fromAscii(sqlStatement->GetColumnString(0).c_str()).toInt();
        }
        sqlStatement->FreeQuery();

        if (!configCount)
        {
            sqlStatement->Sql("INSERT  INTO tb_config(id, terminal_id,ups_status,md5_id,city_id,upload_image,goods_check,version_app) VALUES ( 1, '0000000000000000','true','lxyz123456','101190101','true','false','2014-1-1 00:00:00');");
            sqlStatement->ExecuteAndFree();
           // qDebug() << "insert first config";
        }
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

//增添蔬菜字段表
    QString resultStr;
    sqlStatement->Sql("select sql from  sqlite_master where tbl_name='tb_cell_info' and type='table';" );

    while (sqlStatement->FetchRow())
    {
         resultStr = QString::fromAscii(sqlStatement->GetColumnString(0).c_str());
    }

    sqlStatement->ExecuteAndFree();
    if(!resultStr.contains("is_vegetable"))
    {
        sqlStatement->Sql("Alter TABLE tb_cell_info add column is_vegetable text");
        sqlStatement->ExecuteAndFree();
    }

    return ret;
}


bool CDatBase::setTerminalId(QString terminalId)
{
    bool ret = true;

    try{
       //SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

       // SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);


        sqlStatement->Sql("UPDATE tb_config SET terminal_id=@terminalIdVal WHERE id='1'");
        sqlStatement->BindString(1,terminalId.toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}

QString CDatBase::getTerminalId()
{

    QString terminalId;

    try{
       // SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       //sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("SELECT terminal_id FROM tb_config WHERE id='1'");

        while (sqlStatement->FetchRow())
        {
            terminalId = QString::fromAscii(sqlStatement->GetColumnString(0).c_str());
        }
        sqlStatement->FreeQuery();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
    }

    return terminalId;
}


bool CDatBase::setServerInfo(QString ip, QString port)
{
    bool ret = true;

    try{
       // SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

       // SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("UPDATE tb_config SET server_ip=@serverIp, server_port=@serverPort WHERE id='1'");
        sqlStatement->BindString(1,ip.toStdString());
        sqlStatement->BindString(2,port.toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}


bool CDatBase::getServerInfo(QString *ip, QString *port)
{
    bool ret = true;

    try{
        //SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

       // SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("SELECT server_ip,server_port FROM tb_config WHERE id='1'");

        while (sqlStatement->FetchRow())
        {
            *ip = QString::fromAscii(sqlStatement->GetColumnString(0).c_str());
            *port = QString::fromAscii(sqlStatement->GetColumnString(1).c_str());
        }
        sqlStatement->FreeQuery();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}


bool CDatBase::setUpsInfo(QString upsInfo)
{
    bool ret = true;

    try{
       // SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("UPDATE tb_config SET ups_status=@upsInfo WHERE id='1'");
        sqlStatement->BindString(1,upsInfo.toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}

bool CDatBase::getUpsInfo(QString *upsInfo)
{
    bool ret = true;

    try{
        //SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

       // SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("SELECT ups_status FROM tb_config WHERE id='1'");

        while (sqlStatement->FetchRow())
        {
            *upsInfo = QString::fromAscii(sqlStatement->GetColumnString(0).c_str());
        }
        sqlStatement->FreeQuery();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}


bool CDatBase::setMdCode(QString mdId)
{
    bool ret = true;

    try{
       // SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("UPDATE tb_config SET md5_id=@mdId WHERE id='1'");
        sqlStatement->BindString(1,mdId.toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;

}

QString CDatBase::getMdCode()
{
    QString md5Code;

    try{
       // SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       //sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("SELECT md5_id FROM tb_config WHERE id='1'");

        while (sqlStatement->FetchRow())
        {
            md5Code = QString::fromAscii(sqlStatement->GetColumnString(0).c_str());
        }
        sqlStatement->FreeQuery();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
    }

    return md5Code;
}


bool CDatBase::setCityId(QString cityId)
{
    bool ret = true;

    try{
       // SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("UPDATE tb_config SET city_id=@cityId WHERE id='1'");
        sqlStatement->BindString(1,cityId.toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;

}


QString CDatBase::getCityId()
{
    QString cityId;

    try{
       // SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       //sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("SELECT city_id FROM tb_config WHERE id='1'");

        while (sqlStatement->FetchRow())
        {
            cityId = QString::fromAscii(sqlStatement->GetColumnString(0).c_str());
        }
        sqlStatement->FreeQuery();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
    }

    return cityId;
}

bool CDatBase::setUploadImages(QString loadInfo)
{
    bool ret = true;

    try{
       // SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("UPDATE tb_config SET upload_image=@loadInfo WHERE id='1'");
        sqlStatement->BindString(1,loadInfo.toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}

QString CDatBase::getUploadImages()
{
    QString loadInfo;

    try{
        //SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

       // SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("SELECT upload_image FROM tb_config WHERE id='1'");

        while (sqlStatement->FetchRow())
        {
            loadInfo = QString::fromAscii(sqlStatement->GetColumnString(0).c_str());
        }
        sqlStatement->FreeQuery();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
    }

    return loadInfo;
}

bool CDatBase::setGoodsCheck(QString goodsCheck)
{
    bool ret = true;

    try{
       // SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("UPDATE tb_config SET goods_check=@goodsCheck WHERE id='1'");
        sqlStatement->BindString(1,goodsCheck.toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}

bool CDatBase::getGoodsCheck()
{
    bool ret = false;
    QString goodsCheck;

    try{

        sqlStatement->Sql("SELECT goods_check FROM tb_config WHERE id='1'");

        while (sqlStatement->FetchRow())
        {
            goodsCheck = QString::fromAscii(sqlStatement->GetColumnString(0).c_str());
        }
        sqlStatement->FreeQuery();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
    }

    if(goodsCheck == "true")
        ret = true;

    return ret;
}


bool CDatBase::setAppVersion(QString appInfo)
{
    bool ret = true;

    try{
       // SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("UPDATE tb_config SET version_app=@appInfo WHERE id='1'");
        sqlStatement->BindString(1,appInfo.toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}

QString CDatBase::getAppVersion()
{
    QString version;

    try{
        //SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

       // SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("SELECT version_app FROM tb_config WHERE id='1'");

        while (sqlStatement->FetchRow())
        {
            version = QString::fromAscii(sqlStatement->GetColumnString(0).c_str());
        }
        sqlStatement->FreeQuery();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
    }

    return version;
}



bool CDatBase::setBackgoundImgPath(QString backgroundImg)
{
    bool ret = true;

    try{
       // SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("UPDATE tb_config SET background_img=@backgroundImgIdVal WHERE id='1'");
        sqlStatement->BindString(1,backgroundImg.toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}

QString CDatBase::getBackgoundImgPath()
{

    QString backgroundImg;

    try{
        //SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("SELECT background_img FROM tb_config WHERE id='1'");

        while (sqlStatement->FetchRow())
        {
            backgroundImg = QString::fromAscii(sqlStatement->GetColumnString(0).c_str());
        }
        sqlStatement->FreeQuery();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
    }

    return backgroundImg;
}


bool CDatBase::setBoxCount(qint8 big, qint8 middle, qint8 small,qint8 vegetable)
{
    bool ret = true;

    try{

        sqlStatement->Sql("UPDATE tb_cell_info SET is_big=@big,is_middle=@middle,is_small=@small,is_vegetable=@vegetable WHERE id='1'");
        sqlStatement->BindString(1,QString::number(big).toStdString());
        sqlStatement->BindString(2,QString::number(middle).toStdString());
        sqlStatement->BindString(3,QString::number(small).toStdString());
        sqlStatement->BindString(4,QString::number(vegetable).toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}


bool CDatBase::getBoxCount(CellInfoDesc_t &cellInfo)
{
    bool ret = true;

    try{

        sqlStatement->Sql("SELECT is_big,is_middle,is_small,is_vegetable FROM tb_cell_info WHERE id='1'");

        while (sqlStatement->FetchRow())
        {
            cellInfo.bigCell = QString::fromAscii(sqlStatement->GetColumnString(0).c_str()).toInt();
            cellInfo.middleCell = QString::fromAscii(sqlStatement->GetColumnString(1).c_str()).toInt();
            cellInfo.smallCell = QString::fromAscii(sqlStatement->GetColumnString(2).c_str()).toInt();
            cellInfo.vegetableCell = QString::fromAscii(sqlStatement->GetColumnString(3).c_str()).toInt();
        }
        sqlStatement->FreeQuery();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}

int CDatBase::insertTakePackageInfo(QString delivery, QString opencell, QString getcode, QString celltype)
{
    int ret = -1;
    try{
       // SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

       // SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

        //sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("SELECT * FROM tb_local_save WHERE is_used='false'");
        if ( sqlStatement->FetchRow())
        {
            QString id = QString::fromAscii(sqlStatement->GetColumnString(0).c_str());
            sqlStatement->FreeQuery();


            sqlStatement->Sql("UPDATE tb_local_save SET delivery_id=@delivery, opencell_id=@opencell, cell_type=@celltype, getcode_id=@getcode,is_used = 'true' WHERE id=@id");
            sqlStatement->BindString(1, delivery.toStdString());
            sqlStatement->BindString(2, opencell.toStdString());
            sqlStatement->BindString(3, celltype.toStdString());
            sqlStatement->BindString(4, getcode.toStdString());
            sqlStatement->BindString(5, id.toStdString());
            sqlStatement->ExecuteAndFree();
            ret = id.toInt();
        }
        else
        {
            sqlStatement->FreeQuery();
        }


    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
    }

    return ret;

}


bool CDatBase::queryTakePackageInfo(TakeInfoDesc_t &takeInfo,QString getcode)
{
    bool ret = false;

    try{

        sqlStatement->Sql("SELECT * FROM tb_local_save WHERE is_used='true' and getcode_id=@getcode");
        sqlStatement->BindString(1, getcode.toStdString());

        if(sqlStatement->FetchRow())
        {
            takeInfo.index    = QString::fromAscii(sqlStatement->GetColumnString(0).c_str()).toInt();
            takeInfo.delivery = QString::fromAscii(sqlStatement->GetColumnString(2).c_str());
            takeInfo.getcode  = QString::fromAscii(sqlStatement->GetColumnString(3).c_str());
            takeInfo.opencell = QString::fromAscii(sqlStatement->GetColumnString(4).c_str());
            takeInfo.celltype = QString::fromAscii(sqlStatement->GetColumnString(5).c_str());
            sqlStatement->ExecuteAndFree();

            ret = true;
        }
        else
        {
            sqlStatement->FreeQuery();
        }



    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
    }

    return ret;
}


bool CDatBase::deleteTakePackageInfo(QString deliveryId)
{
    bool ret = true;

    try{

        sqlStatement->Sql("UPDATE tb_local_save SET is_used='false' WHERE delivery_id=@deliveryId");
        sqlStatement->BindString(1,deliveryId.toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}



bool CDatBase::deleteTakePackageCellInfo(QString opencell)
{
    bool ret = true;

    try{

        sqlStatement->Sql("UPDATE tb_local_save SET is_used='false' WHERE opencell_id=@opencell");
        sqlStatement->BindString(1,opencell.toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}

void CDatBase::deleteTable()
{
    sqlStatement->Sql("DROP TABLE [tb_local_save]");
    sqlStatement->ExecuteAndFree();
}


int CDatBase::insertOpenException(qint8 cabinetId, qint8 cellId,qint8 celltype,QString errType)
{
    int ret = -1;
    try{
       // SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

       // SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

        //sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("SELECT * FROM tb_open_exception WHERE is_used='false'");
        if ( sqlStatement->FetchRow())
        {
            QString id = QString::fromAscii(sqlStatement->GetColumnString(0).c_str());
            sqlStatement->FreeQuery();


            sqlStatement->Sql("UPDATE tb_open_exception SET cabinet_id=@cabinetId, cell_id=@cellId, cell_type=@celltype, errType=@errType,is_used = 'true' WHERE id=@id");
            sqlStatement->BindString(1, QString::number(cabinetId).toStdString());
            sqlStatement->BindString(2, QString::number(cellId).toStdString());
            sqlStatement->BindString(3, QString::number(celltype).toStdString());
            sqlStatement->BindString(4, errType.toStdString());
            sqlStatement->BindString(5, id.toStdString());
            sqlStatement->ExecuteAndFree();
            ret = id.toInt();
        }
        else
        {
            sqlStatement->FreeQuery();
        }


    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
    }

    return ret;

}

void CDatBase::queryOpenException(QList<CellDesc_t> & cells)
{
    cells.clear();

    try{
       // SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

        //sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("SELECT * FROM tb_open_exception WHERE is_used='true'");
        while ( sqlStatement->FetchRow())
        {
            CellDesc_t cellDesc;
            cellDesc.index = QString::fromAscii(sqlStatement->GetColumnString(0).c_str()).toInt();
            cellDesc.cabinetId = QString::fromAscii(sqlStatement->GetColumnString(2).c_str()).toInt();
            cellDesc.cellId = QString::fromAscii(sqlStatement->GetColumnString(3).c_str()).toInt();
            cellDesc.cellType = QString::fromAscii(sqlStatement->GetColumnString(4).c_str()).toInt();
            cellDesc.errType = QString::fromAscii(sqlStatement->GetColumnString(5).c_str());
            cells.append(cellDesc);
        }
        sqlStatement->FreeQuery();

    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
    }

}



bool CDatBase::setOpenException(qint8 index)
{
    bool ret = true;

    try{
        //SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("UPDATE tb_open_exception SET errType='1' WHERE id=@index");
        sqlStatement->BindString(1,QString::number(index).toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}

bool CDatBase::deleteOpenException(qint8 index)
{
    bool ret = true;

    try{
       // SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

        //sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("UPDATE tb_open_exception SET is_used='false' WHERE id=@index");
        sqlStatement->BindString(1,QString::number(index).toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}

//欠费
int CDatBase::insertDebtException(QString deliveryId, qint8 debt)
{
    int ret = -1;
    try{
        //SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

        //sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("SELECT * FROM tb_debt_exception WHERE is_used='false'");
        if ( sqlStatement->FetchRow())
        {
            QString id = QString::fromAscii(sqlStatement->GetColumnString(0).c_str());
            sqlStatement->FreeQuery();

            sqlStatement->Sql("UPDATE tb_debt_exception SET deliveryId=@deliveryId, debtValue=@debt,is_used = 'true' WHERE id=@id");
            sqlStatement->BindString(1, deliveryId.toStdString());
            sqlStatement->BindString(2, QString::number(debt).toStdString());
            sqlStatement->BindString(3, QString(id).toStdString());
            sqlStatement->ExecuteAndFree();
            ret = id.toInt();
        }
        else
        {
             sqlStatement->FreeQuery();
        }


    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
    }

    return ret;
}


void CDatBase::queryDebtException(QList<DebtDesc_t> & cells)
{
    cells.clear();
    try{
       // SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

        //sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);
        sqlStatement->Sql("SELECT * FROM tb_debt_exception WHERE is_used='true'");
        while ( sqlStatement->FetchRow())
        {
            DebtDesc_t debtDesc;
            debtDesc.index = QString::fromAscii(sqlStatement->GetColumnString(0).c_str()).toInt();
            debtDesc.deliveryId = QString::fromAscii(sqlStatement->GetColumnString(2).c_str());
            debtDesc.debt = QString::fromAscii(sqlStatement->GetColumnString(3).c_str()).toInt();
            cells.append(debtDesc);
        }
        sqlStatement->FreeQuery();

    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
    }

}


void CDatBase::setDebtException(qint8 index,qint8 debt)
{

    try{
        //SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("UPDATE tb_debt_exception SET debtValue=@debt WHERE id=@index");
        sqlStatement->BindString(1,QString::number(debt).toStdString());
        sqlStatement->BindString(2,QString::number(index).toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() <<"e.GetString().c_str():" << e.GetString().c_str();
    }

}


bool CDatBase::deleteDebtException(qint8 index)
{
    bool ret = true;

    try{
        //SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("UPDATE tb_debt_exception SET is_used='false' WHERE id=@index");
        sqlStatement->BindString(1,QString::number(index).toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}





int CDatBase::insertServerException(QString url, QString param,QString box,QString errType)
{
    int ret = -1;
    try{
        //SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

        //sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("SELECT * FROM tb_server_exception WHERE is_used='false'");
        if ( sqlStatement->FetchRow())
        {
            QString id = QString::fromAscii(sqlStatement->GetColumnString(0).c_str());
            sqlStatement->FreeQuery();


            sqlStatement->Sql("UPDATE tb_server_exception SET interface_name=@url, param=@param, exit_door=@box, errType =@errType,is_used = 'true' WHERE id=@id");
            sqlStatement->BindString(1, url.toStdString());
            sqlStatement->BindString(2, param.toStdString());
            sqlStatement->BindString(3, box.toStdString());
            sqlStatement->BindString(4, errType.toStdString());
            sqlStatement->BindString(5, QString(id).toStdString());
            sqlStatement->ExecuteAndFree();
            ret = id.toInt();
        }
        else
        {
             sqlStatement->FreeQuery();
        }


    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
    }

    return ret;

}

void CDatBase::queryServerException(QList<ServerDesc_t> &serverWaitInfo)
{
    serverWaitInfo.clear();

    try{
        //SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

        //sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("SELECT * FROM tb_server_exception WHERE is_used='true'");
        while ( sqlStatement->FetchRow())
        {
            ServerDesc_t serverDesc;
            serverDesc.index = QString::fromAscii(sqlStatement->GetColumnString(0).c_str()).toInt();
            serverDesc.url = QString::fromAscii(sqlStatement->GetColumnString(2).c_str());
            serverDesc.param = QString::fromAscii(sqlStatement->GetColumnString(3).c_str());
            serverDesc.cellId = QString::fromAscii(sqlStatement->GetColumnString(4).c_str());
            serverDesc.errType = QString::fromAscii(sqlStatement->GetColumnString(5).c_str());
            serverWaitInfo.append(serverDesc);
        }
        sqlStatement->FreeQuery();

    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
    }


}


bool CDatBase::deleteServerException(qint8 index)
{
    bool ret = true;

    try{
        //SQLiteDatabase *sqliteDb = new SQLiteDatabase(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        //SQLiteStatement *sqlStatement = new SQLiteStatement(sqliteDb);

       // sqliteDb->Open(mDbPath.toStdString(), SQLITE_OPEN_READWRITE, 0);

        sqlStatement->Sql("UPDATE tb_server_exception SET is_used='false' WHERE id=@index");
        sqlStatement->BindString(1,QString::number(index).toStdString());
        sqlStatement->ExecuteAndFree();
    }
    catch(SQLiteException& e){
        qDebug() << e.GetString().c_str();
        ret = false;
    }

    return ret;
}
