#ifndef MCONFIG_H
#define MCONFIG_H

#include <QObject>
#include "singleton.h"

class QString;

class mconfig : public QObject
{
    Q_OBJECT
public:
    explicit mconfig(QObject *parent = 0);

    bool writePingIP(QString tempStr);
    void readPingIP(QString &tempStr);

    bool writeNetworklevel(QString tempStr);
    void readNetworklevel(QString &tempStr);

    bool writeNetworkConf(QString tempIpaddrStr,QString tempNetmaskStr,QString tempDefaultGwStr,QString tempDNSStr1,QString tempDNSStr2);
    void readNetworkConf(QString &tempIpaddrStr,QString &tempNetmaskStr,QString &tempDefaultGwStr,QString &tempDNSStr1,QString &tempDNSStr2);

private:
    void initXml();
    void creadXml();

signals:
    
public slots:

private:

    QString  mPingIPStr;//ping ip
    QString  mNetworklevelStr;//network level
    QString  mNetworkIpaddrStr;
    QString  mNetworkNetmaskStr;
    QString  mNetworkDefaultGwStr;
    QString  mNetworkDNSStr1;
    QString  mNetworkDNSStr2;

    DECLARE_SINGLETON_CLASS(mconfig)
    
};
typedef Singleton<mconfig> Smconfig;
#endif // MCONFIG_H
