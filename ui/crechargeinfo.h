#ifndef CRECHARGEINFO_H
#define CRECHARGEINFO_H

#include <QDialog>

class QLineEdit;
class QPushButton;
class QTimer;
class QFrame;

class CRechargeInfo : public QDialog
{
    Q_OBJECT
public:
    explicit CRechargeInfo(QWidget *parent = 0);

    void readYearInfo(QString name,QString mobilePhone,QString m1Card,QString first,QString second);

    void readTraiffInfo(QString name, QString mobilePhone,QString m1Card, QString price);
    
signals:
    void rechargeInfo_Ok();
public slots:
    void closeRechargePage();
private:
    void initPage();

    QLineEdit *mNameLineEdit;
    QLineEdit *mMobilePhoneLineEdit;
    QLineEdit *mM1CardLineEdit;

    QLineEdit *mPriceLineEdit;

    QLineEdit *mCardDateLineEdit;
    QLineEdit *mTopDateLineEdit;

    QTimer *timer;

    QFrame *yearFrame;
    QFrame *tariffFrame;
};

#endif // CRECHARGEINFO_H
