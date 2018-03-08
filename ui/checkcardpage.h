#ifndef CHECKCARDPAGE_H
#define CHECKCARDPAGE_H

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>

class checkcardpage : public QDialog
{
    Q_OBJECT
public:
    explicit checkcardpage(QWidget *parent = 0);

    void readCardInfo(QString data);

private:

    void initPage();
    QPushButton *exitBtn;
    QLineEdit   *lineEdit;
    
signals:
    
public slots:

    void exitBtn_slots();
    
};

#endif // CHECKCARDPAGE_H
