#ifndef CPROMPTINFO_H
#define CPROMPTINFO_H

#include <QDialog>

class QTimer;

namespace Ui {
class CPromptInfo;
}

class CPromptInfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit CPromptInfo(QWidget *parent = 0);
    ~CPromptInfo();

    void showIcPromptPage();

public slots:
    void closeICPromptPage();
    
signals:
    void logRequest();

private:
    Ui::CPromptInfo *ui;
    QTimer *timer;
};

#endif // CPROMPTINFO_H
