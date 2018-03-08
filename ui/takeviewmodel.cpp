#include "takeviewmodel.h"

takeViewModel::takeViewModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}


int takeViewModel::rowCount(const QModelIndex &parent) const
{
    return mDataList.size();
}

int takeViewModel::columnCount(const QModelIndex &parent) const
{
    return mHorList.size();
}


void takeViewModel::setHorizontalHeaderList(QStringList hlist)
{
    mHorList = hlist;
}

QVariant takeViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            return mHorList[section];
        }

        return QVariant();
    }

    return QVariant();
}

QVariant takeViewModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    if(role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    if(role == Qt::DisplayRole)
    {
        return mDataList.at(index.row()).at(index.column());
    }

    return QVariant();
}


bool takeViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;

    if(role == Qt::EditRole)
    {
        QStringList list = mDataList.at(index.row());

        list.replace(index.column(),value.toString());

        mDataList.replace(index.row(),list);
       //mDataList.value(index.row()).value(index.column()) = value;
        emit dataChanged(index,index);
        return true;
    }

    return false;
}


void takeViewModel::setModelData(QList<QStringList> &datalist)
{
    mDataList.clear();
    mDataList = datalist;
    //qDebug() << mDataList;
}

void takeViewModel::refrushModel()
{
    beginResetModel();
    endResetModel();
}
