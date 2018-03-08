/*********************************************************************
* 版权所有 (C)2013,  南京蓝芯电力技术。
*
* 文件名称： threadsocket.cpp
* 文件标识：
* 内容摘要： socket线程，单个线程管理socket的收发实现
* 其它说明：
* 当前版本：
* 作   者：
* 完成日期： 2013/08/07
*
**********************************************************************/


#include "tsocket.h"


using namespace Tcp;

TSocket::TSocket(int socketDescriptor, QTcpSocket *socket)
{
    mDescriptor = socketDescriptor;
    mQSocket = socket;


}


TSocket::~TSocket()
{
    mQSocket->close();
    delete mQSocket;

    quit();
    wait();
    deleteLater();
}



void TSocket::run()
{
   // qDebug()<<QThread::currentThreadId() << "Tsocket";
    //绑定接收数据信号的槽
    connect(this->mQSocket,SIGNAL(readyRead()),this,SLOT(slotReadyRead()));
    //exec();
}

void TSocket::write(QByteArray& data)
{
    mQSocket->write(data);
}


void TSocket::slotReadyRead()
{
    emit signalOnData(mDescriptor, mQSocket->readAll());
}
