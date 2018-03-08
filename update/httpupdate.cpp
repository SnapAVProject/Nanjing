#include "httpupdate.h"
#include "md5.h"

#include <QUuid>
#include <QRegExp>
#include <QDebug>
#include <istream>

httpUpdate::httpUpdate(QObject *parent) :
    QObject(parent)
{
}


bool httpUpdate::getAppInfo(QString url, QString updateName, QString terminal, QString md5Code,QString fileMd5)
{
    bool ret = false;

    nam = new QNetworkAccessManager(this);


    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    url = url + "fileFileName=" + updateName +"&terminalId="+terminal +"&encryptCode=" + md5Code +"&randomUUID="+strUuId;
    QNetworkRequest        request;
    request.setUrl(QUrl(url));

    updateName = "./file/" + updateName;
    QFile f(updateName);
    if (f.open(QIODevice::WriteOnly))
    {
        reply = nam->get(request);
        QEventLoop loop;//事件机制，变为同步
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        loop.exec();// 进入事件循环， 直到reply的finished()信号发出， 这个语句才能结束

        readRespone(reply,f);
    }

    MD5 md5;
    md5.reset();
    std::ifstream file(updateName.toAscii());
    md5.update(file);
    if(md5.toString().c_str() == fileMd5)
    {
        ret = true;
    }

    delete nam;
    return ret;
}

void httpUpdate::readRespone(QNetworkReply *reply,QFile &f)
{
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();


    if (status == 200)
    {
        f.write(reply->readAll());
        f.close();
    }
    reply->close();
    delete reply;
}
