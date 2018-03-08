#ifndef URLENCODE_H
#define URLENCODE_H

#include <QString>
#include <QNetworkReply>
#include <QMutex>
#include <QEventLoop>
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QDataStream>
#include <QUuid>

#define BOUNDARY  "---------------------------7dd3123a0674"

class Http :public QObject
{
    Q_OBJECT
public:
    /**********************************************************************
    * 函数名称： HttpSync
    * 功能描述： 构造函数
    * 输入参数：
    * 输出参数：
    * 修改日期        版本号     修改人	      修改内容
    * -----------------------------------------------
    * 2013/08/07	 V1.0
    ***********************************************************************/
   Http(){}

   /**********************************************************************
   * 函数名称： post
   * 功能描述： 向后台发送请求
   * 输入参数：
   * 输出参数：
   * 修改日期        版本号     修改人	      修改内容
   * -----------------------------------------------
   * 2013/08/07	 V1.0
   ***********************************************************************/
   QString postFile(QString url, QString file,QString md5Code)
    {
        QNetworkAccessManager* nam = new QNetworkAccessManager();
        QNetworkReply*         reply;
        QString                respone;
        QString                fileName = file.split('/').last();
        QNetworkRequest        request;
        QByteArray             param;

        QString temp = fileName.split('.').first() + "\r\n";
        QString md5CodeTemp = md5Code + "\r\n";

        QUuid uuId = QUuid::createUuid();
        QString strUuId = uuId.toString();
        strUuId = strUuId.remove(QRegExp("[{}-]"));
        strUuId = strUuId.toUpper() + "\r\n";

        request.setUrl(QUrl(url));

       // qDebug() <<

        param =  "--" + QByteArray( BOUNDARY) + "\r\n";
        param += "Content-Disposition: form-data; name=\"deliveryId\"\r\n\r\n";
        param += temp;
        param +=  "--" + QByteArray( BOUNDARY) + "\r\n";
        param += "Content-Disposition: form-data; name=\"encryptCode\"\r\n\r\n";
        param += md5CodeTemp;
        param +=  "--" + QByteArray( BOUNDARY) + "\r\n";
        param += "Content-Disposition: form-data; name=\"randomUUID\"\r\n\r\n";
        param += strUuId;
       // param += "abc\r\n";
        param += "--" + QByteArray( BOUNDARY) + "\r\n";
        param += "Content-Disposition: form-data; name=\"dePhotoFile\"; filename=\""+ fileName + "\"\r\n";
        param += "Content-Type: image/jpeg\r\n\r\n";


        QFile f(file);
        if (f.open(QIODevice::ReadOnly))
        {
            QDataStream in(&f);
            int length = f.size();
            char* buffer = new char[length];
            in.readRawData(buffer,length);
            QByteArray img = QByteArray(buffer, length);
            param += img;
            param +="\r\n";
            f.close();

           /*
            QFile fsave("a.jpg");
            if (fsave.open(QIODevice::ReadWrite))
            {
                QDataStream out(&fsave);
                out.writeRawData(buffer, length);
            }
            fsave.close();

            system("rm a.jpg");
            */

          //  qDebug() << buffer;
            delete buffer;
        }

        param += "--" + QByteArray(BOUNDARY) + "--\r\n";


        request.setRawHeader("Accept", "textml, application/xhtml+xml, */*");
        request.setRawHeader("Referer", "http://192.168.1.77:8080/lxyz/upload.jsp");
        request.setRawHeader("Accept-Language", "zh-CN");
        request.setRawHeader("User-Agent", "Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; WOW64; Trident/6.0; MALCJS)");
        request.setRawHeader("Content-Type", "multipart/form-data; boundary=" + QByteArray(BOUNDARY));
        request.setRawHeader("Accept-Encoding", "gzip, deflate");
        request.setRawHeader("Content-Length", QString( param.size()).toAscii());
        request.setRawHeader("DNT", "1");

        request.setRawHeader("Connection", "Keep-Alive");
        request.setRawHeader("Cache-Control", "no-cache");
        request.setRawHeader("Cookie", "JSESSIONID=119802BAE40F52725DBE210859459BD5");
        //request.setHeader(QNetworkRequest::ContentTypeHeader,QByteArray("multipart/form-data; boundary=")+BOUNDARY);
        //request.setHeader(QNetworkRequest::ContentLengthHeader,param.size());


        reply = nam->post(request, param);
        QEventLoop loop;//事件机制，变为同步
        connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop,  SLOT(quit()));
        loop.exec();// 进入事件循环， 直到reply的finished()信号发出， 这个语句才能结束

        respone = reply->readAll();
        reply->close();
        reply->deleteLater();
/*


            request.setRawHeader("Content-Type", QByteArray("multipart/form-data; boundary=")+BOUNDARY);

            QByteArray param =  QByteArray( BOUNDARY) + "\r\n";
            param += "Content-Disposition: form-data; name=\"deliveryId\"\r\n\r\n";
            param += "676\r\n";

             param +=  QByteArray( BOUNDARY) + "\r\n";
            param += "ontent-Disposition: form-data; name=\"dePhotoFile\"; filename=\""+fileName+"\"\r\n";
            param += "Content-Type: text/plain\r\n";  //image/png




            request.setRawHeader("Content-Length", QString(param.size()).toAscii() );




        readRespone(post_reply,respone);*/


     //   qDebug() << respone;
        delete nam;
        return respone;
    }

public slots:
    /**********************************************************************
    * 函数名称： httperror
    * 功能描述： 处理链接错误
    * 输入参数：
    * 输出参数：
    * 修改日期        版本号     修改人	      修改内容
    * -----------------------------------------------
    * 2013/08/07	 V1.0
    ***********************************************************************/
    void httperror(QNetworkReply::NetworkError error)
    {
        qDebug()<<"httperror:"<<error;
    }

private:
    /**********************************************************************
    * 函数名称： readRespone
    * 功能描述： 读取后台返回回来的数据
    * 输入参数：
    * 输出参数：
    * 修改日期        版本号     修改人	      修改内容
    * -----------------------------------------------
    * 2013/08/07	 V1.0
    ***********************************************************************/
    void readRespone(QNetworkReply*reply,QString& respone)
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



};

#endif // URLENCODE_H
