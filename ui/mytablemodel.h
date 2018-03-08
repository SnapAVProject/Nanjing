#ifndef MYTABLEMODEL_H
#define MYTABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

class CFormTablemodel : public QAbstractTableModel
{
public:
    CFormTablemodel();
    QStringList header;
    QStringList datalist;
    void setheader(QStringList list);
    void setdata(QStringList list);
    void refrushModel();
protected:

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // MYTABLEMODEL_H
