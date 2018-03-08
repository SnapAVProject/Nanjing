/*********************************************************************
* 版权所有 (C)2013,  南京蓝芯电力技术。
*
* 文件名称： lxparser.h
* 文件标识：
* 内容摘要： QT程序与驱动进程间socket通信协议数据解析
*           ---------------------------------------------------------------------------------
*           |  Header (2) | Index(1) | Command(1) | Length(1) | Data(n) | CHECK(1) | End(1) |
*           +-------------+----------+------------+-----------+---------+----------+--------+
*           |  0x4c 0x58  |    a     |     b      |      n    |         |     x    |  0x45  |
*           +--------------------------------------------------------------------------------
*
* 其它说明：
* 当前版本：
* 作   者： shiwanlin
* 完成日期： 2013/08/07
*
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
**********************************************************************/

#ifndef LXPARSER_H
#define LXPARSER_H

#include <server.h>
#include <driver.h>
#include <singleton.h>


typedef enum {
    LX_OPEN_LOCK  = 0x01,
    LX_OPEN_LOCK_RESP,
    LX_QUERY_LOCK,
    LX_QUERY_LOCK_RESP,
    LX_QUERY_GOODS,
    LX_QUERY_GOODS_RESP,
    LX_KEY_IPUT = 0x12,
    LX_M1_CARD_INPUT = 0x22,
    LX_CODE_SCAN = 0x31,
    LX_CODE_SCAN_RESP,
    LX_COIN_INPUT = 0x42,
    LX_SCREEN    = 0x51,
    LX_SCREEN_RESP ,
    LX_UPS_INPUT = 0x72
}LX_CMD;


class LxProtocolParser : public Tcp::Observer
{
public:

    /**********************************************************************
    * 函数名称： LxProtocolParser
    * 功能描述： LxProtocolParser 构造函数
    * 输入参数： lxDrv  ： 数据帧信息接收者
    * 输出参数：
    * 修改日期        版本号     修改人	      修改内容
    * -----------------------------------------------
    * 2013/08/07	 V1.0
    ***********************************************************************/
    LxProtocolParser()
    {
        mParserSt = ST_WAIT_FLAG_4C;
    }

    /**********************************************************************
    * 函数名称： ~LxProtocol
    * 功能描述： LxProtocol 析构函数
    * 输入参数：
    * 输出参数：
    * 修改日期        版本号     修改人	      修改内容
    * -----------------------------------------------
    * 2013/08/07	 V1.0
    ***********************************************************************/
    ~LxProtocolParser()
    {
    }

    /**********************************************************************
    * 函数名称： onDataProc
    * 功能描述： 接收数据处理函数，解析数据帧，解析完成后通知mLxDrvr
    * 输入参数： data ： 本次接收到数据包
    * 输出参数：
    * 修改日期        版本号     修改人	      修改内容
    * -----------------------------------------------
    * 2013/08/07	 V1.0
    ***********************************************************************/
    void onDataProc(QByteArray data)
    {
        qDebug() << "input data : " <<  data.toHex() ;

        for (int i = 0; i < data.size(); i++)
            onCharProc(data[i]);
       /* QByteArray test;
        test.append(0x01);
        test.append(0x02);
        test.append(0x01);
        mLxDrvr->onCommandProc(0x01, 0x02,  test);*/
    }


private:

    /**********************************************************************
    * 函数名称： onCharProc
    * 功能描述： 数据解析字符状态机
    * 输入参数： c ： 本次接收到字符
    * 输出参数：
    * 修改日期        版本号     修改人	      修改内容
    * -----------------------------------------------
    * 2013/08/07	 V1.0
    ***********************************************************************/
    void onCharProc(qint8 c)
    {

        switch (mParserSt)
        {
            case ST_WAIT_FLAG_4C:
                if ( 0x4c == c)
                    mParserSt = ST_WAIT_FLAG_58;
                break;
            case ST_WAIT_FLAG_58:
                if (0x58 == c)
                {
                    mParserSt = ST_WAIT_INDEX;
                    mCheck = 0;
                    mCurPos = 0;
                    mLength = 0;
                }
                else
                {
                    mParserSt = ST_WAIT_FLAG_4C;
                }
                break;
            case ST_WAIT_INDEX:
                mIndex = c;
                mParserSt = ST_WAIT_CMD;
                mCheck += c;
                break;
            case ST_WAIT_CMD:
                mCommand = c;
                mCheck += c;
                mParserSt = ST_WAIT_LENGTH;
                break;
            case ST_WAIT_LENGTH:
                mLength = c;
                mCheck += c;
                if (!mLength)
                {
                    mParserSt = ST_WAIT_CHECK;
                }
                else
                {

                    mParserSt = ST_WAIT_DATA;
                }
                break;
            case ST_WAIT_DATA:
                mData[mCurPos++] = c;
                mCheck += c;
                if (mCurPos >= mLength)
                {
                    mParserSt = ST_WAIT_CHECK;
                }
                break;
            case ST_WAIT_CHECK:
                if (mCheck == c)
                {
                    mParserSt = ST_WAIT_FLAG_45;
                }
                else
                {
                    mParserSt = ST_WAIT_FLAG_4C;
                }
                break;
            case ST_WAIT_FLAG_45:
                if (c == 0x45)
                {
                    //处理数据帧
                    QByteArray data((const char*)mData, mLength);
                    SCDriver::instance()->onCommandProc(mIndex, mCommand,  data);                 

                }
                mParserSt = ST_WAIT_FLAG_4C;
                break;
            default:
                break;
            }
    }

private:

    typedef enum {
        ST_WAIT_FLAG_4C, //初始状态，等待接收0x4c
        ST_WAIT_FLAG_58, //已经接收到0x4c，等待接收0x58
        ST_WAIT_INDEX,   //等待结算INDEX
        ST_WAIT_CMD,     //已经接收到INDEX，等待接收CMD
        ST_WAIT_LENGTH,  //已经接收到CMD，等待接收LENGTH
        ST_WAIT_DATA,    //已经接收到LENGTH，接收数据
        ST_WAIT_CHECK,   //数据接收完成，等待接收校验码
        ST_WAIT_FLAG_45  //等待接收结束标识
    }PARSER_ST;

    PARSER_ST  mParserSt; //字符状态机当前状态
    qint8      mCommand;  //接收到命令符
    qint8      mLength;   //数据区长度
    qint8      mCurPos;   //当前接收到数据位置
    qint8      mData[256];//接收到的数据缓冲
    qint8      mCheck;    //校验累加和
    qint8      mIndex;

};


class LxProtocolGenerater
{

public :
    /**********************************************************************
    * 函数名称： generate
    * 功能描述： 生成LxProtocol协议数据
    * 输入参数： command : 控制命令符
    *          data    ： 本次接收到数据包
    * 输出参数：
    * 修改日期        版本号     修改人	      修改内容
    * -----------------------------------------------
    * 2013/08/07	 V1.0
    ***********************************************************************/

    static QByteArray generate(LX_CMD command, QByteArray data);


};


#endif // LXPARSER_H
