#ifndef EXCEPTIONOPENDOORPAGE_H
#define EXCEPTIONOPENDOORPAGE_H

#include <QDialog>
#include <QComboBox>
#include "mymessage.h"
#include "lockdrv.h"

class QPushButton;

namespace Ui {
class exceptionopendoorpage;
}

class exceptionopendoorpage : public QDialog
{
    Q_OBJECT
    
public:
    explicit exceptionopendoorpage(QWidget *parent = 0);
    ~exceptionopendoorpage();

signals:
    void resetTime();

public slots:
    void okBtn_slots();
    void cancelBtn_slots();

private:
    Ui::exceptionopendoorpage *ui;

    void initPage();

    void maintainPage();

    bool filterCell(QString openCell);

    CLockDrv lockoperation;

    QComboBox *cabinetComBox;
    QComboBox *cellComBox;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

    CMessageBox myMessageBox;
};

#endif // EXCEPTIONOPENDOORPAGE_H
