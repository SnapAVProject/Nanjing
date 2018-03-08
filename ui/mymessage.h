#ifndef MYMESSAGE_H
#define MYMESSAGE_H
#include <QDialog>
#include <QTimer>
#include <QMessageBox>


namespace Ui {
class mymessage;
}

class CMessageBox : public QDialog
{
    Q_OBJECT
    
public:
    explicit CMessageBox(QWidget *parent = 0);
    explicit CMessageBox(long time,QWidget *parent = 0);
    ~CMessageBox();

signals:
    void message_ok();
    void message_cancel();
    void message_close();

public slots:
    void stopTimer();

public:
    void setCheckTimer(long time = 50*1000);
    void myPrompt(QMessageBox::Icon,const QString & title, const QString & text,Qt::WindowModality model = Qt::WindowModal);
private:
    void initPage();
    Ui::mymessage *ui;
    QTimer timer;
};

#endif // MYMESSAGE_H
