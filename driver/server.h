/*********************************************************************
* 版权所有 (C)2013,  南京蓝芯电力技术。
*
* 文件名称： tcpserver.h
* 文件标识：
* 内容摘要： QT程序与驱动Tcp服务
* 其它说明：
* 当前版本：
* 作   者：
* 完成日期： 2013/08/07
*
* 修改记录1：
*    修改日期：
*    版 本 号：
*    修 改 人：
*    修改内容：
**********************************************************************/

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QList>
#include "tsocket.h"


namespace  Tcp {


    class Observer{
    public:
        Observer(){}
        ~Observer(){}


        /**********************************************************************
        * 函数名称： onDataProc
        * 功能描述： tcp接收数据观察者接收处理存虚函数
        * 输入参数： QByteArray ： 接收的数据
        * 输出参数：
        * 修改日期        版本号     修改人	      修改内容
        * -----------------------------------------------
        * 2013/08/07	 V1.0
        ***********************************************************************/
        virtual void onDataProc(QByteArray data) = 0;
    };


    class Server : public QObject
    {
        Q_OBJECT
    public:

        /**********************************************************************
        * 函数名称： Server
        * 功能描述： Server构造函数
        * 输入参数：
        * 输出参数：
        * 修改日期        版本号     修改人	      修改内容
        * -----------------------------------------------
        * 2013/08/07	 V1.0
        ***********************************************************************/
        explicit Server(QObject *parent = 0);

        /**********************************************************************
        * 函数名称： ~Server
        * 功能描述： Server析构函数，删除链接和监听套接字释放所有链接
        * 输入参数：
        * 输出参数：
        * 修改日期        版本号     修改人	      修改内容
        * -----------------------------------------------
        * 2013/08/07	 V1.0
        ***********************************************************************/
        ~Server();

        /**********************************************************************
        * 函数名称： start
        * 功能描述： 启动TCP服务
        * 输入参数： port  ： 监听的端口
        * 输出参数：
        * 修改日期        版本号     修改人	      修改内容
        * -----------------------------------------------
        * 2013/08/07	 V1.0
        ***********************************************************************/
        void start(int port);

        /**********************************************************************
        * 函数名称： write
        * 功能描述： 向所有客户端发送数据
        * 输入参数： data  ： 发送的数据
        * 输出参数：
        * 修改日期        版本号     修改人	      修改内容
        * -----------------------------------------------
        * 2013/08/07	 V1.0
        ***********************************************************************/
        void write(QByteArray& data);

        /**********************************************************************
        * 函数名称： addObserver
        * 功能描述： 添加接收数据观察者
        * 输入参数： observer  ： 收数据观察者
        * 输出参数：
        * 修改日期        版本号     修改人	      修改内容
        * -----------------------------------------------
        * 2013/08/07	 V1.0
        ***********************************************************************/
        void addObserver(Observer* observer);

    signals:

    private slots:
        /**********************************************************************
        * 函数名称： slotNewConnection
        * 功能描述： 建立链接事件处理槽，把链接放入链接池
        * 输入参数：
        * 输出参数：
        * 修改日期        版本号     修改人	      修改内容
        * -----------------------------------------------
        * 2013/08/07	 V1.0
        ***********************************************************************/
        void  slotNewConnection();


        /**********************************************************************
        * 函数名称： slotSocketError
        * 功能描述： socket错误事件处理槽
        * 输入参数： socketError socket 错误原因
        * 输出参数：
        * 修改日期        版本号     修改人	      修改内容
        * -----------------------------------------------
        * 2013/08/07	 V1.0
        ***********************************************************************/
        void  slotSocketError(QAbstractSocket::SocketError socketError);

        /**********************************************************************
        * 函数名称： slotOnData
        * 功能描述： 接收数据槽处理，被接收线程调用
        * 输入参数： socketDescriptor 接收socket标识
        *          data             接收到数据
        * 输出参数：
        * 修改日期        版本号     修改人	      修改内容
        * -----------------------------------------------
        * 2013/08/07	 V1.0
        ***********************************************************************/
        void  slotOnData(int socketDescriptor,QByteArray data);
    private:
        QTcpServer*         mListen;  //监听套接字
        QMap<int, TSocket*> mSockets; //链接线程池
        QList<Observer*>    mObservers;//观察者
    };
}






#endif // TCPSERVER_H