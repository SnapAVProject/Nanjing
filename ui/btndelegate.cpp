#include "btndelegate.h"
#include <qapplication.h>
#include <QDebug>

BtnDelegate::BtnDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

    connect(&myMessageBox,SIGNAL(message_ok()),this,SLOT(sendClick_slots()));
}


void BtnDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(index.column() == 5)
    {
        const QAbstractItemModel *itemModel = index.model();

        QString finishedSubTaskNum = itemModel->data(index, Qt::DisplayRole).toString();
        QStyleOptionButton *buttonOption = new QStyleOptionButton();
        buttonOption->rect = option.rect;
        buttonOption->state|= QStyle::State_Enabled | QStyle::State_AutoRaise;
        buttonOption->iconSize = QSize(120,40);
        if(finishedSubTaskNum == "点击取出")
        {
            QIcon icon(QPixmap(":/page/images/exception/takePressBtn.png"));
            buttonOption->icon = icon;
        }
        else if(finishedSubTaskNum == "已取出")
        {
            QIcon icon(QPixmap(":/page/images/exception/open.png"));
            buttonOption->icon = icon;
        }
        else if(finishedSubTaskNum == "打不开")
        {
            QIcon icon(QPixmap(":/page/images/exception/notOpen.png"));
            buttonOption->icon = icon;
        }
//        QColor color(64,128,128);
//        QColor color2(192,192,192);
//        if(finishedSubTaskNum == "点击取出")
//        {
//            buttonOption->palette = QPalette(color);
//        }
//        else
//        {
//            buttonOption->palette = QPalette(color2);
//        }

      //  buttonOption->text = finishedSubTaskNum;
       // buttonOption->textVisible = true;

        QApplication::style()->drawControl(QStyle::CE_PushButton, buttonOption, painter);

//        if(finishedSubTaskNum == "点击取出")
//        {
//            QApplication::style()->drawItemPixmap(painter,buttonOption->rect,Qt::AlignCenter,QPixmap(":/page/images/exception/takePressBtn.png"));
//        }

    }
    else
    {
        QStyledItemDelegate::paint(painter,option, index);
    }
}


QSize BtnDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(index.column() == 5)
    {    
      return QSize(120,40);
    }
    else
    {
        return QStyledItemDelegate::sizeHint(option, index);
    }

}


bool BtnDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonRelease)
    {
     //   qDebug() << model->data(index,Qt::DisplayRole) << index.row() << index.column();

        if(model->data(index,Qt::DisplayRole).toString() == QString("点击取出"))
        {
            row = index.row();
            coloum = index.column();

            QModelIndex indexA = model->index(index.row(), index.column() - 2, QModelIndex());
            QModelIndex indexB = model->index(index.row(), index.column() - 1, QModelIndex());
            QString scabid = model->data(indexA,Qt::DisplayRole).toString();
            QString scell = model->data(indexB,Qt::DisplayRole).toString();

            myMessageBox.myPrompt(QMessageBox::Warning,tr("提示确认"),QString("您确定打开%1柜%2号箱门？").arg(scabid).arg(scell),Qt::NonModal);
        }
        return true;
    }
    return false;
}

void BtnDelegate::sendClick_slots()
{
    emit btnClick(row,coloum);

}
