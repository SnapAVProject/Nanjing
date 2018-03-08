#ifndef SAVEPACKAGELISTPAGE_H
#define SAVEPACKAGELISTPAGE_H

#include <QDialog>
#include "mytablemodel.h"
#include "servercomm.h"
#include <QPushButton>
#include <QTableView>
#include <QProgressDialog>

class savepackagelistpage : public QDialog
{
    Q_OBJECT
public:
    savepackagelistpage();
    void readInfo(QByteArray);

    int daemonListSize();
    void setNotOpenBox(QMap<QString,qint8>);
    void setNotCheckBox(QMap<QString,qint8>);
    void setNotCloseBox(QMap<QString,qint8>);

public slots:
    void okBtnSlots();
    void nextBtnSlots();

signals:
    void timeStop();
private:
    void initListInfopage();
    void setBtnEnable(bool);

    QTableView *savetableView;
    CFormTablemodel *showModel;
    QPushButton *nextBtn;
    QPushButton *okBtn;
   // stockShowInfo stockInfo;                    //存件的清单信息
   // packInfo daemonInfo;                        //发送给后台的数据


    long packageNumber;
    QString M1Id;
    QStringList stockList;
    QList<packInfo>daemonList;                  //发送给后台的数据
    QMap<QString,qint8> notOpenBoxList;         //无法打开的箱门列表
    QMap<QString,qint8> notCheckBoxList;        //物品检测损坏的的箱门列表
    QMap<QString,qint8> notCloseBox;            //未关闭的箱门

    QProgressDialog progress;

};

#endif // SAVEPACKAGELISTPAGE_H
