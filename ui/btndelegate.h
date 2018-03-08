#ifndef BTNDELEGATE_H
#define BTNDELEGATE_H

#include <QStyledItemDelegate>
#include "mymessage.h"

class BtnDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit BtnDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option,const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:

    void btnClick(int,int);
    
public slots:
    void sendClick_slots();

private:
    CMessageBox myMessageBox;
    int row,coloum;
    
};

#endif // BTNDELEGATE_H
