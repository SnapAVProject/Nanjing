#ifndef CADVERTISEMENT_H
#define CADVERTISEMENT_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class CAdvertisement;
}

class CAdvertisement : public QDialog
{
    Q_OBJECT
    
public:
    explicit CAdvertisement(QWidget *parent = 0);
    ~CAdvertisement();

    void advShowPage(QString);

signals:
    void advPageClose();
    
private:
    Ui::CAdvertisement *ui;
    QTimer timer;

    void initPage();
};

#endif // CADVERTISEMENT_H
