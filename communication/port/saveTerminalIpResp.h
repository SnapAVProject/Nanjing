#ifndef SAVETERMINALIPRESP_H
#define SAVETERMINALIPRESP_H

#include <QObject>
#include <QString>

class saveTerminalIpResp:public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success           READ success      WRITE setSuccess)
    Q_PROPERTY(QString errCode           READ errCode      WRITE setErrcode)
    Q_PROPERTY(QString errMessage        READ errMessage   WRITE setErrMessage)
    Q_PROPERTY(QString reportCycle       READ reportCycle  WRITE setReportCycle)

public:

    saveTerminalIpResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString errMessage() const{return merrMessage;}
    QString reportCycle() const{return mReportCycle;}

    ~saveTerminalIpResp()
    {
    }

    void setSuccess(QString success)
    {
        mSuccess = success;
    }
    void setErrcode(QString errCode)
    {
        mErrcode = errCode;
    }
    void setErrMessage(QString errMessage)
    {
        merrMessage = errMessage;
    }
    void setReportCycle(QString reportCycle)
    {
        mReportCycle = reportCycle;
    }



private:
    QString mSuccess ;
    QString mErrcode;
    QString merrMessage;
    QString mReportCycle;
};

#endif // SAVETERMINALIPRESP_H
