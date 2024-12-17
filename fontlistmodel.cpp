#include "fontlistmodel.h"

FontListModel::FontListModel(QObject *parent)
    : QAbstractListModel(parent)
{}


int FontListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    Q_UNUSED(parent);
    qDebug() << "Rown count: " << fontList.size();
    return fontList.size();
}


QVariant FontListModel::data(const QModelIndex &index, int role) const
{
    qDebug() << "Index: " << index.row();
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    if (role == Qt::DisplayRole) {
        qDebug() << "Item at: " << index.row() << QString(fontList.at(index.row()).getC());
        return QString(fontList.at(index.row()).getC());
        // return "Test";
    }
    return QVariant();
}

bool FontListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        emit dataChanged(index, index, {role});
        return true;
    }
    return true;
}

void FontListModel::insertRow(int row, Font font)
{
    beginInsertRows(QModelIndex(), row, row);
    fontList.insert(row, font);
    endInsertRows();
}


