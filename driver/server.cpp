#include "server.h"
#include <QDebug>

using namespace Tcp;

Server::Server( QObject *parent) :
    QObject(parent)
{
}


Server::~Server()
{
    QMap<int, TSocket*>::iterator it;

    //删除所有的客户端链接
    for (it = mSockets.begin(); it!= mSockets.end(); it++)
        delete it.value();
}

void Server::start(int port)
{
    mListen = new QTcpServer;
    mListen->listen(QHostAddress::Any, port);
    QObject::connect(mListen,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));

    qDebug() << "start tcp server " <<" port : " << port;
}

void Server::write(QByteArray& data)
{
    QMap<int, TSocket*>::iterator it;

    qDebug() << " send data: " << data.toHex();

    for (it = this->mSockets.begin(); it != this->mSockets.end(); ++it)
        it.value()->write(data);
}


void Server::slotNewConnection()
{

    QTcpSocket* socket = mListen->nextPendingConnection();

    TSocket* t = new TSocket(socket->socketDescriptor(), socket);

    QObject::connect(t,SIGNAL(signalOnData(int, QByteArray)),this,SLOT(slotOnData(int, QByteArray)), Qt::DirectConnection);

    mSockets.insert(socket->socketDescriptor(), t);

    t->start();

    qDebug()<<"currentThreadId :" << QThread::currentThreadId();

//    qDebug() << "new connection " << ":" << socket->socketDescriptor();
}


void Server::slotSocketError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "socketError :" << socketError;
}


void Server::slotOnData(int socketDescriptor, QByteArray data )
{
    for (int i = 0 ; i < mObservers.size(); i++)
        mObservers[i]->onDataProc(data);
}


void Server::addObserver(Observer *observer)
{
    mObservers.append(observer);
}
