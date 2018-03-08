#ifndef INFOINPUTPAGE_H
#define INFOINPUTPAGE_H

#include <QDialog>
#include "mylineedit.h"
#include "mymessage.h"
#include "servercomm.h"
#include <QComboBox>
#include <QStackedLayout>
#include <QMap>
#include <QProgressDialog>

class QGraphicsView;
class QCheckBox;

namespace Ui {
    class infoinputpage;
}

class infoinputpage : public QDialog {
    Q_OBJECT
public:
    infoinputpage(QWidget *parent = 0);
    ~infoinputpage();

    void readInfo(QByteArray);

signals:
    void resetTime();

public slots:
    void okBtn_slots();
    void cancelBtn_slots();

    void checkBox_slots(int);
    void stateOkBtn_slots();

    void readCardInfo(QString);

protected:
    void changeEvent(QEvent *e);
    bool eventFilter(QObject *, QEvent *);

private slots:
    void hideCompany_slots(const QString &);

private:
    Ui::infoinputpage *ui;

    QStackedLayout *stack;

    QGraphicsView *textContent;
    QCheckBox *checkBox;
    QCheckBox *opposeCheckBox;

    QPushButton *stateOkBtn;
    QPushButton *stateCancelBtn;

    QComboBox *mCompanyComBox;
    QLabel *companyLabel;

    QPushButton *okBtn;
    QPushButton *cancelBtn;
    CKeyBoardLineEdit *mPhoneLineEdit;
    CKeyBoardLineEdit *mIdLineEdit;
    CKeyBoardLineEdit *mM1CardLineEdit;
    //CKeyBoardLineEdit *mCustomerTypeLineEdit;
    QComboBox *mCustomerTypeComBox;

    CMessageBox myMessageBox;

    QString optM1;

    bool isM1IdWorking;
    QStringList mCompanyIdInfo;
    QStringList mCompanyNameInfo;
    QList<ExpressCompany> mCompanyList;

    QProgressDialog progress;

    void initStatePage();
    void initPage();
    bool judgeInfo();
    void clearInfo();
};

#endif // INFOINPUTPAGE_H
