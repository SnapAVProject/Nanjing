#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QDialog>
#include <QLineEdit>
#include <QString>
#include <QKeyEvent>




namespace Ui {
    class keyboard;
}



class keyboard : public QDialog {
    Q_OBJECT
public:
    keyboard(QWidget *parent = 0);
    ~keyboard();

public:
    QString result;
    void setLineEditData(QString);

signals:
    void dataChange();
    void focusChange();

    void btn0signal();
    void btn1signal();
    void btn2signal();
    void btn3signal();
    void btn4signal();
    void btn5signal();
    void btn6signal();
    void btn7signal();
    void btn8signal();
    void btn9signal();
    void btnXsignal();
    void btnspacesignal();
    void btnwellsignal();
    void backbtnsignal();
    void okbtnsignal();
    void cancelbtnsignal();

public slots:
    void btn0slot();
    void btn1slot();
    void btn2slot();
    void btn3slot();
    void btn4slot();
    void btn5slot();
    void btn6slot();
    void btn7slot();
    void btn8slot();
    void btn9slot();
    void btnXslot();
    void btnspaceslot();
    void btnwellslot();
    void backbtnslot();
    void okbtnslot();
    void cancelbtnslot();

protected:
    void changeEvent(QEvent *e);
   // void keyPressEvent(QKeyEvent *);

private:
    Ui::keyboard *ui;
    void initpage();
    QString lineEditData;
};

#endif // KEYBOARD_H
