#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QEventLoop>
#include <QByteArray>
#include <QDebug>
#include <QTimer>
#include "httpsync.h"

HttpSync::HttpSync()
{
    isWorking = false;
}

/**********************************************************************
* 函数名称： post
* 功能描述： 向后台发送请求
* 输入参数：
* 输出参数：
* 修改日期        版本号     修改人	      修改内容
* -----------------------------------------------
* 2013/08/07	 V1.0
***********************************************************************/
QString HttpSync::post(QString url, QString  param)
{

    if (!isWorking)
    {
        isWorking = true;
        respone.clear();
        nam = new QNetworkAccessManager();

        QNetworkRequest request;
        request.setUrl(QUrl(url));



    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::ContentLengthHeader,QByteArray::number(param.toAscii().size()));


    post_reply = nam->post(request, param.toAscii());

    QTimer timer;
    timer.setSingleShot(true);
    timer.setInterval(30000);
    timer.start();
    QEventLoop loop;//事件机制，变为同步
    connect(post_reply, SIGNAL(finished()), &loop, SLOT(quit()));
    connect(&timer,SIGNAL(timeout()),&loop,SLOT(quit()));
    connect(post_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
               SLOT(httperror(QNetworkReply::NetworkError)));
    loop.exec();// 进入事件循环， 直到reply的finished()信号发出， 这个语句才能结束

    timer.stop();
    readRespone(post_reply,respone);
        delete nam;

	isWorking = false;
    }

    return respone;
}


/**********************************************************************
* 函数名称： readRespone
* 功能描述： 读取后台返回回来的数据
* 输入参数：
* 输出参数：
* 修改日期        版本号     修改人	      修改内容
* -----------------------------------------------
* 2013/08/07	 V1.0
***********************************************************************/

void HttpSync::readRespone(QNetworkReply *reply,QString& respone)
{
    respone = "";
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();


    if (status == 200)
    {
        respone = reply->readAll();
    }

    reply->close();
    reply->deleteLater();
}





void HttpSync::httperror(QNetworkReply::NetworkError error)
{
    qDebug()<<"httperror:"<<error;
}
