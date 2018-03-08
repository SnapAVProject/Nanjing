#ifndef CTAKEOPENCELL_H
#define CTAKEOPENCELL_H

#include <QDialog>
#include <QTimer>
#include <QLabel>

class CTakeOpenCell : public QDialog
{
    Q_OBJECT
public:
    explicit CTakeOpenCell(QWidget *parent = 0);
    void initPage();
    void setCellLab(QString data);
    void setCellLab(QString data,double payMoney,double balance);
    
signals:
    void displayOpenCell();
    
public slots:
    void timerOver();

private:
    QTimer timer;
    QLabel *doorId;
    QLabel *doorCellId;
    QLabel *payLabel;
    
};

#endif // CTAKEOPENCELL_H
