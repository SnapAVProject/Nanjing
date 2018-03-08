#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QDialog>
#include "cpromptinfo.h"

namespace Ui {
class HomePage;
}

class HomePage : public QDialog
{
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = 0);
    ~HomePage();

    void inithomepage();
    void setBtnEnable(bool status);

    void show_promptPage();

    void upDataCellInfo();

    void getWeather();


public slots:
    void on_storagePushButton_clicked();//存件
    void on_pickupPushButton_clicked();//取
    void on_rechangePushButton_clicked();//充值
    void on_setupPushButton_clicked();//设置
    void on_resgistrationPushButton_clicked();//注册
    void on_blanceQueryPushButton_clicked();//资费查询
    void on_priceCheckPushButton_clicked();//余额查询
    void on_useProcessPushButton_clicked();//流程信息
    void on_introductionPushButton_clicked();//易邮简介
    void on_exceHandPushButton_clicked();//异常处理

    void noicLog_clicked();          //无卡登陆
    void close_promptPage();            //关闭提示框

    void showCurTime();                //当前时间
    bool showWeather();
public:
    Ui::HomePage *ui;
    CPromptInfo *promptInfo;

    QTimer *timer;

    QStringList weekDayList;

    bool weatherStatus;

    int mMinutes;

};

#endif // HOMEPAGE_H
