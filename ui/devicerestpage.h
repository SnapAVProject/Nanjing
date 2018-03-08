#ifndef DEVICERESTPAGE_H
#define DEVICERESTPAGE_H

#include <QDialog>

namespace Ui {
class devicerestpage;
}

class devicerestpage : public QDialog
{
    Q_OBJECT
    
public:
    explicit devicerestpage(QWidget *parent = 0);
    ~devicerestpage();

    void startQueryDevice();

public slots:
    void timeOver();
    
private:
    Ui::devicerestpage *ui;

    QTimer *timer;

     void initPage();
};

#endif // DEVICERESTPAGE_H
