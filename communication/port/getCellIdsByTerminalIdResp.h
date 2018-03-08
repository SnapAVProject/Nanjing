#ifndef GETCELLIDSBYTERMINALIDRESP_H
#define GETCELLIDSBYTERMINALIDRESP_H

//存件时弹出来的箱体

#include <QObject>
#include <QStringList>


class getCellIdsByTerminalIdResp : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString success                    READ success               WRITE setSuccess)
    Q_PROPERTY(QString errCode                    READ errCode               WRITE setErrcode)
    Q_PROPERTY(QString errMessage                 READ errMessage            WRITE setErrMessage)
    Q_PROPERTY(QStringList bigCellIdList          READ bigCellIdList         WRITE setbigCellIdList)
    Q_PROPERTY(QStringList middleCellIdList       READ middleCellIdList      WRITE setmiddleCellIdList)
    Q_PROPERTY(QStringList smallCellIdList        READ smallCellIdList       WRITE setsmallCellIdList)
    Q_PROPERTY(QStringList vegetablesCellidList   READ vegetablesCellidList  WRITE setvegetablesCellidList)

public:

    getCellIdsByTerminalIdResp(QObject *parent = 0): QObject(parent){}

    QString success() const{return mSuccess;}
    QString errCode() const{return mErrcode;}
    QString errMessage() const{return merrMessage;}
    QStringList bigCellIdList() const{return mbigCellIdList;}
    QStringList middleCellIdList() const{return mmiddleCellIdList;}
    QStringList smallCellIdList() const{return msmallCellIdList; }
    QStringList vegetablesCellidList() const{return mvegetablesCellidList; }

    ~getCellIdsByTerminalIdResp()
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

    void setbigCellIdList(QStringList bigCellIdList)
    {
        for(int i = 0;i < bigCellIdList.size(); i++)
        {
            mbigCellIdList.append(bigCellIdList.at(i));
        }
    }

    void setmiddleCellIdList(QStringList middleCellIdList)
    {
        for(int i = 0;i < middleCellIdList.size();i++)
        {
             mmiddleCellIdList.append(middleCellIdList.at(i));
        }
    }

    void setsmallCellIdList(QStringList smallCellIdList)
    {
        for (int i = 0; i < smallCellIdList.size(); i++)
        {
            msmallCellIdList.append(smallCellIdList.at(i));
        }
    }

    void setvegetablesCellidList(QStringList vegetablesCellidList)
    {
        for (int i = 0; i < vegetablesCellidList.size(); i++)
        {
            mvegetablesCellidList.append(vegetablesCellidList.at(i));
        }
    }

private:
    QString mSuccess ;
    QString mErrcode;
    QString merrMessage;
    QStringList mbigCellIdList ;
    QStringList mmiddleCellIdList ;
    QStringList msmallCellIdList;
    QStringList mvegetablesCellidList;
};

#endif // GETCELLIDSBYTERMINALIDRESP_H
