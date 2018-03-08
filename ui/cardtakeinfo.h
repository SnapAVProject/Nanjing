#ifndef CARDTAKEINFO_H
#define CARDTAKEINFO_H

#include <QDialog>
#include <QTimer>

class QLabel;
class cardtakeinfo : public QDialog
{
    Q_OBJECT
public:
    explicit cardtakeinfo(QWidget *parent = 0);

    void setInfo(double,double,double,double,bool role = true);
    
signals:
    void closeInfoPage();
public slots:
    void timerOver();

private:
    void initPage();

    QTimer timer;
    QLabel *takeInfoLabel;
    QLabel *payLabel;
    
};

#endif // CARDTAKEINFO_H
