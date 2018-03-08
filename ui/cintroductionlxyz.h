#ifndef CINTRODUCTIONLXYZ_H
#define CINTRODUCTIONLXYZ_H

#include <QDialog>
class QStackedLayout;
class CIntroductionLXYZ : public QDialog
{
    Q_OBJECT
public:
    explicit CIntroductionLXYZ(QDialog *parent = 0);

public slots:
    void exitPage();
private:
    void initPage();
    
};

#endif // CINTRODUCTIONLXYZ_H
