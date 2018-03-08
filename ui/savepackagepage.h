#ifndef SAVEPACKAGEPAGE_H
#define SAVEPACKAGEPAGE_H

#include <QDialog>
#include <QTimer>
#include "mylineedit.h"
#include "mytablemodel.h"
#include <QToolButton>
#include "servercomm.h"
#include "mymessage.h"
#include <QLabel>
#include <QPushButton>
#include "lockdrv.h"

class QButtonGroup;

typedef enum{
    NO_ST = 0x00,
    BIG_ST,
    MID_ST,
    SMA_ST,
    VEG_ST
}BTN_STATUS;



namespace Ui {
    class savepackagepage;
}

class savepackagepage : public QDialog {
    Q_OBJECT
public:
    savepackagepage(QWidget *parent = 0);
    ~savepackagepage();

    void readdisplayinfo(QByteArray param);

    bool loadcodeScan();                       //开始扫描条形码
    void setBtnEable(bool);                    //设置按钮可不可用

signals:
    void resetTime();

public slots:
    void okslots();
    void cancelslots();
    void buttonclickslots(int);

protected:
    void changeEvent(QEvent *e);
    bool eventFilter(QObject *, QEvent *);

private:
    Ui::savepackagepage *ui;

    CLockDrv lockoperation ;

    QToolButton *smallBoxBtn;
    QToolButton *middleBoxBtn;
    QToolButton *bigBoxBtn;
    QToolButton *vegetableBtn;

    CKeyBoardLineEdit *barCodeLineEdit;
    CKeyBoardLineEdit *phoneLineEdit;
    CKeyBoardLineEdit *againPhoneLineEdit;

    QLabel *balanceLabel;

    QLabel *bigBoxLab;            //显示箱子剩余量
    QLabel *middleBoxLab;
    QLabel *smallBoxLab;
    QLabel *vegetableLab;

    QLabel *bigCheckedLab;        //显示是否选中的
    QLabel *middleCheckedLab;
    QLabel *smallCheckedLab;
    QLabel *vegetableCheckedLab;

    QPushButton *okBtn;
    QPushButton *cancelBtn;

    bool bigstatus;                //箱子的状态
    bool middlestatus;
    bool smallstatus;
    bool vegetablestatus;

    BTN_STATUS Cur_Status;             //当前选中状态



    void initPackageInfo();
    void updatebox();                          //更新箱体数目
    void initlist(QByteArray);                 //初始化箱体
    void filterCell();                         //过滤箱体是否已经用过
    void checkIsFixCellInfo();

    bool inputCheck();                         //检测信息是否正确  0：失败 1：成功
    bool openSelectLocks(QStringList*);        //打开箱门，重新选择打开箱门
    void btnconfirminfo();                     //录入箱体按钮确认信息
    void enterboxstatus(BTN_STATUS);           //按钮状态
    void packdata(QByteArray &);               //打包数据
    void clearInfo();
    void setEableEdit(bool);                   //设置窗口能否被编辑





   // QByteArray data;                            //箱子的信息
    QStringList bigCellList;                    //大号箱体信息
    QStringList middleCellList;                 //中号箱体信息
    QStringList smallCellList;                  //小号箱体信息
    QStringList vegetableCellList;              //蔬菜箱体信息
    QStringList orderBigCellList;               //承包大号箱体信息
    QStringList orderMiddleCellList;            //承包中号箱体信息
    QStringList orderSmallCellList;             //承包小号箱体信息
    QStringList orderVegetableCellList;         //承包蔬菜箱体信息
    QStringList orderCellInfos;                 //所有承包箱体信息


    QMap<QString,qint8> notOpenBoxList;         //无法打开的箱门列表
    QMap<QString,qint8> notCheckBoxList;        //物品检测损坏的的箱门列表
    packInfo daemonInfo;                        //发送给后台的数据
    QString M1Id;                               //M1卡卡号
    QString terminal;                           //终端编号

   // qint8 cabinetId;                             //柜号
  //  qint8 cellId;                                //箱号

    CMessageBox myMessageBox;

    QStringList blackNameList;                  //黑名单信息

 };

#endif // SAVEPACKAGEPAGE_H
