#include "mytablemodel.h"


//***************************************************************************
/**file		mytablemodel.cpp
* brief		模型，显示出取件包裹列表
* author	hjzhang@163.com	2013/07/25
* version	0.1
*/
//***************************************************************************
#include "mytablemodel.h"
#include <QDebug>

CFormTablemodel::CFormTablemodel()
{
  //  datalist = list;
   // qDebug()<<datalist;
    //header<<"序号"<<"快递公司"<<"快递单号"<<"箱号"<<"欠费";

}

void CFormTablemodel::setheader(QStringList list)
{
    header = list;
 // emit QAbstractItemModel::headerDataChanged(Qt::Horizontal,0,0);
}

void CFormTablemodel::setdata(QStringList list)
{
  //  header<< "序号" << "快递单号" << "用户手机号" << "柜号" << "箱号";
    datalist = list;

    //emit QAbstractItemModel::dataChanged(0, 0);
    //  qDebug()<<datalist;

}


int CFormTablemodel::rowCount(const QModelIndex &) const
{

  //  qDebug()<<datalist.count()<<header.count();
    return datalist.count()/header.count();
}

int CFormTablemodel::columnCount(const QModelIndex &) const
{
    return header.count();
}

QVariant CFormTablemodel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if( Qt::TextAlignmentRole == role )
               return Qt::AlignCenter;
    if(role == Qt::DisplayRole )
    {
      //  qDebug()<<"he"<<index.row()<<index.column();
        return datalist[header.count()*index.row()+index.column()];
    }
    else
        return QVariant();

}

QVariant CFormTablemodel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    if(role == Qt::DisplayRole && orientation == Qt::Vertical)
    {
        return "";
    }
    return QAbstractTableModel::headerData(section,orientation,role);

}

void CFormTablemodel::refrushModel()
{
    beginResetModel();
    endResetModel();
}
