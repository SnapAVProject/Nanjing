#ifndef SENDIMAGE_H
#define SENDIMAGE_H

#include <QThread>
#include <QDir>
#include "urlencode.h"
#include "singleton.h"
#include "servercomm.h"
#include "mainctrl.h"
#include "db.h"

#define SLEEP_TIME  3*60

class CSendImageThrd : QThread
{
    Q_OBJECT

public:

    void init()
    {
        start();
    }

    void setSendImage()
    {
        status = !status;
    }

    bool readSendImageStatus()
    {
        return status;
    }

    void setIpPort(QString ip,QString port)
    {
        mStrIP = ip;
        mStrPort = port;
    }

private:
    CSendImageThrd(){}
    ~CSendImageThrd(){}


    bool status;
    QString mStrIP;
    QString mStrPort;


    void sendImageProc()
    {
        QDir dir("/usr/lxyz/picture/");


        QString url = "http://" + mStrIP +":" + mStrPort +"/lxyz/soapManager_uploadDeliveryPhoto.do";
        QStringList nameFilter;
        nameFilter << "*.gif";
        QStringList images = dir.entryList(nameFilter,QDir::NoFilter, QDir::Time);

        for (int i = 0; i < images.size(); i++)
        {
            QString imagefile = "/usr/lxyz/picture/" + images[i];

            QFile filesize(imagefile);
            if(filesize.size() > 0)
            {
                uploadDeliveryPhotoResp respone;
                SCServerComm::instance()->uploadDeliveryPhoto(url,imagefile,&respone);

                if(respone.success() == "true")
                {
                  // qDebug() << " start delete image" << "image:" << images[i];
                   images[i] = "./picture/" + images[i];
                   QString temp = QString("rm %1").arg(images[i]);
                   QByteArray imageData = temp.toLatin1();
                   system(imageData.data());
                }
            }
            else
            {
               images[i] = "./picture/" + images[i];
               QString temp = QString("rm %1").arg(images[i]);
               QByteArray imageData = temp.toLatin1();
               system(imageData.data());
            }

        }

    }

    void run()
    {
        QString uploadImage = SCDatBase::instance()->getUploadImages();
        if(uploadImage == "true")
        {
            status = true;
        }
        else
        {
            status = false;
        }
        SCDatBase::instance()->getServerInfo(&mStrIP,&mStrPort);

        while(1)
        {
            sleep(SLEEP_TIME);
            if (SCMainCtrl::instance()->curStatus() == 1)
            {
                if(status)
                {
                    //qDebug() << "start send image";
                    sendImageProc();
                }
            }

        }
    }

    DECLARE_SINGLETON_CLASS(CSendImageThrd)
};

typedef Singleton<CSendImageThrd> SCSendImageThrd;
#endif // SENDIMAGE_H
