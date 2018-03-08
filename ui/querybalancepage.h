#ifndef QUERYBALANCEPAGE_H
#define QUERYBALANCEPAGE_H

#include <QDialog>
#include "mylineedit.h"
#include "mymessage.h"

namespace Ui {
class querybalancepage;
}

class querybalancepage : public QDialog
{
    Q_OBJECT
    
public:
    explicit querybalancepage(QWidget *parent = 0);
    ~querybalancepage();

signals:
    void resetTime();

public slots:
    void okBtn_slots();
    void cancelBtn_slots();
    void readCardInfo(QString);

private:
    Ui::querybalancepage *ui;

    CKeyBoardLineEdit *idLineEdit;

    QPushButton *cancelBtn;
    QPushButton *okBtn;

    CMessageBox myMessageBox;

    void initPage();
};

#endif // QUERYBALANCEPAGE_H
