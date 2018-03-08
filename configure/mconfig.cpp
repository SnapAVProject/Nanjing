#include "mconfig.h"
#include <QFile>
#include <QtXml/QDomDocument>
#include <QDebug>
mconfig::mconfig(QObject *parent) :
    QObject(parent)
{
    initXml();
}

void mconfig::initXml()
{

    mPingIPStr.clear();
    mNetworklevelStr.clear();
    mNetworkIpaddrStr.clear();
    mNetworkNetmaskStr.clear();
    mNetworkDefaultGwStr.clear();

    QFile file("config.xml");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        creadXml();
        file.open(QFile::ReadOnly | QFile::Text);
    }
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << " mconfig initXml setContent fail";
        file.close();
        return ;
    }
    file.close();

     QDomElement root = doc.documentElement();
     QDomNode node = root.firstChild();
     while (!node.isNull())
     {
         if (node.toElement().tagName() == "pingIP")
         {
             mPingIPStr = node.firstChild().toText().data();
         }
         else if (node.toElement().tagName() == "network")
         {
             QDomNode childNode = node.firstChild();
             while (!childNode.isNull())
             {
                 if (childNode.toElement().tagName() == "level")
                 {
                     mNetworklevelStr = childNode.firstChild().toText().data();
                 }
                 else if (childNode.toElement().tagName() == "ipaddr")
                 {
                     mNetworkIpaddrStr = childNode.firstChild().toText().data();
                 }
                 else if (childNode.toElement().tagName() == "netmask")
                 {
                     mNetworkNetmaskStr = childNode.firstChild().toText().data();
                 }
                 else if (childNode.toElement().tagName() == "defaultGw")
                 {
                     mNetworkDefaultGwStr = childNode.firstChild().toText().data();
                 }
                 else if (childNode.toElement().tagName() == "dns1")
                 {
                     mNetworkDNSStr1 = childNode.firstChild().toText().data();
                 }
                 else if (childNode.toElement().tagName() == "dns2")
                 {
                     mNetworkDNSStr2 = childNode.firstChild().toText().data();
                 }
                 childNode = childNode.nextSibling();//读取兄弟节点
             }
         }
         node = node.nextSibling();//读取兄弟节点
     }
}

void mconfig::creadXml()
{
    QDomDocument doc;
    QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);
    QDomElement root = doc.createElement("configure");
    doc.appendChild(root);


    QDomElement note = doc.createElement("pingIP");
    root.appendChild(note);
    QDomText nodeText = doc.createTextNode("www.baidu.com");
    note.appendChild(nodeText);

    QDomElement no = doc.createElement("network");
    root.appendChild(no);

    QDomElement level = doc.createElement("level");
    no.appendChild(level);
    QDomText levelText = doc.createTextNode("0");
    level.appendChild(levelText);

    QDomElement ipaddr = doc.createElement("ipaddr");
    no.appendChild(ipaddr);
    QDomText ipaddrText = doc.createTextNode("192.168.1.100");
    ipaddr.appendChild(ipaddrText);

    QDomElement netmask = doc.createElement("netmask");
    no.appendChild(netmask);
    QDomText netmaskText = doc.createTextNode("255.255.255.0");
    netmask.appendChild(netmaskText);

    QDomElement DefaultGw = doc.createElement("defaultGw");
    no.appendChild(DefaultGw);
    QDomText DefaultGwText = doc.createTextNode("192.168.1.1");
    DefaultGw.appendChild(DefaultGwText);

    QDomElement DNS1 = doc.createElement("dns1");
    no.appendChild(DNS1);
    QDomText DNS1Text = doc.createTextNode("192.168.1.1");
    DNS1.appendChild(DNS1Text);

    QDomElement DNS2 = doc.createElement("dns2");
    no.appendChild(DNS2);
    QDomText DNS2Text = doc.createTextNode("114.114.114.114");
    DNS2.appendChild(DNS2Text);



    QFile file("config.xml");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
    {
        qDebug() << "mconfig creadXml fail";
        return ;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    doc.save(out,4,QDomNode::EncodingFromTextStream);
    file.close();
}
bool mconfig::writePingIP(QString tempStr)
{
    QFile file("config.xml");
    if (!file.open(QIODevice::ReadOnly |QIODevice::Text))
    {
        qDebug() << "writePingIP open ReadOnly fail";
        return false;
    }
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "writePingIP setContent fail";
        file.close();
        return false;
    }
    file.close();

     QDomElement root = doc.documentElement();
     QDomNode node = root.firstChild();
     while (!node.isNull())
     {
         if (node.toElement().tagName() == "pingIP")
         {
             node.firstChild().setNodeValue(tempStr);
         }
         node = node.nextSibling();//读取兄弟节点
     }


     if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
     {
         qDebug() << "writePingIP open WriteOnly fail";
         file.close();
         return false;
     }
     QTextStream out(&file);
     out.setCodec("UTF-8");
     doc.save(out,4,QDomNode::EncodingFromTextStream);
     file.close();

     mPingIPStr = tempStr;
     return true;
}
void mconfig::readPingIP(QString &tempStr)
{
    tempStr = mPingIPStr;
}

bool mconfig::writeNetworklevel(QString tempStr)
{
    QFile file("config.xml");
    if (!file.open(QIODevice::ReadOnly |QIODevice::Text))
    {
        qDebug() << "writeNetworklevel open ReadOnly fail";
        return false;
    }
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "writeNetworklevel setContent fail";
        file.close();
        return false;
    }
    file.close();

     QDomElement root = doc.documentElement();
     QDomNode node = root.firstChild();
     while (!node.isNull())
     {
         if (node.toElement().tagName() == "network")
         {
             QDomNode childNode = node.firstChild();
             while (!childNode.isNull())
             {
                 if (childNode.toElement().tagName() == "level")
                 {
                     childNode.firstChild().setNodeValue(tempStr);
                 }
                 childNode = childNode.nextSibling();//读取兄弟节点
             }
         }
         node = node.nextSibling();//读取兄弟节点
     }


     if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
     {
         qDebug() << "writeNetworklevel open WriteOnly fail";
         file.close();
         return false;
     }
     QTextStream out(&file);
     out.setCodec("UTF-8");
     doc.save(out,4,QDomNode::EncodingFromTextStream);
     file.close();

     mNetworklevelStr = tempStr;

     return true;
}
void mconfig::readNetworklevel(QString &tempStr)
{
    tempStr = mNetworklevelStr;
}

bool mconfig::writeNetworkConf(QString tempIpaddrStr,QString tempNetmaskStr,QString tempDefaultGwStr,QString tempDNSStr1,QString tempDNSStr2)
{
    QFile file("config.xml");
    if (!file.open(QIODevice::ReadOnly |QIODevice::Text))
    {
        qDebug() << "writeNetworkConf open ReadOnly fail";
        return false;
    }
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "writeNetworkConf setContent fail";
        file.close();
        return false;
    }
    file.close();

     QDomElement root = doc.documentElement();
     QDomNode node = root.firstChild();
     while (!node.isNull())
     {
         if (node.toElement().tagName() == "network")
         {
             QDomNode childNode = node.firstChild();
             while (!childNode.isNull())
             {
                 if (childNode.toElement().tagName() == "ipaddr")
                 {
                     childNode.firstChild().setNodeValue(tempIpaddrStr);
                 }
                 else if(childNode.toElement().tagName() == "netmask")
                 {
                     childNode.firstChild().setNodeValue(tempNetmaskStr);
                 }
                 else if(childNode.toElement().tagName() == "defaultGw")
                 {
                     childNode.firstChild().setNodeValue(tempDefaultGwStr);
                 }
                 else if(childNode.toElement().tagName() == "dns1")
                 {
                     childNode.firstChild().setNodeValue(tempDNSStr1);
                 }
                 else if(childNode.toElement().tagName() == "dns2")
                 {
                     childNode.firstChild().setNodeValue(tempDNSStr2);
                 }
                 childNode = childNode.nextSibling();//读取兄弟节点
             }

         }
         node = node.nextSibling();//读取兄弟节点
     }


     if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
     {
         qDebug() << "writeNetworkConf open WriteOnly fail";
         file.close();
         return false;
     }
     QTextStream out(&file);
     out.setCodec("UTF-8");
     doc.save(out,4,QDomNode::EncodingFromTextStream);
     file.close();

     mNetworkIpaddrStr = tempIpaddrStr;
     mNetworkNetmaskStr = tempNetmaskStr;
     mNetworkDefaultGwStr = tempDefaultGwStr;
     mNetworkDNSStr1 = tempDNSStr1;
     mNetworkDNSStr2 = tempDNSStr2;

     return true;
}
void mconfig::readNetworkConf(QString &tempIpaddrStr, QString &tempNetmaskStr, QString &tempDefaultGwStr, QString &tempDNSStr1, QString &tempDNSStr2)
{
    tempIpaddrStr = mNetworkIpaddrStr;
    tempNetmaskStr = mNetworkNetmaskStr;
    tempDefaultGwStr = mNetworkDefaultGwStr;
    tempDNSStr1 = mNetworkDNSStr1;
    tempDNSStr2 = mNetworkDNSStr2;
}
