#include "fontslistmodel.h"
#include <QDebug>

FontsListModel::FontsListModel(QList<FontPixelMap> fonts, QObject *parent)
    : QAbstractListModel(parent), fonts(fonts)
{

}

int FontsListModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    Q_UNUSED(parent);
    return fonts.size();

    // FIXME: Implement me!
}

QVariant FontsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (role == Qt::DecorationRole)
        return QPixmap::fromImage(fonts.at(index.row()).getFontBitmap().toImage().scaled(80, 80));
    if (role == Qt::DisplayRole && index.row() < fonts.size())
        return QVariant::fromValue(fonts.at(index.row()).getCharacter());

    return QVariant();
}


FontPixelMap FontsListModel::getData(int index) const
{
    return fonts.at(index);
}

void FontsListModel::insertRow(int row, FontPixelMap font)
{
    beginInsertRows(QModelIndex(), row, row);
    fonts.insert(row, font);
    endInsertRows();
}

void FontsListModel::updateRow(int row, FontPixelMap font)
{
    if (row < 0 || row >= fonts.size()) {
        qDebug() << "Invalid row index:" << row;
        return;
    }
    fonts[row] = font;
    emit dataChanged(index(row, 0), index(row, 0));
}

QList<FontPixelMap> FontsListModel::getAllData()
{
    return fonts;
}

void FontsListModel::removeAll()
{
    beginRemoveRows(QModelIndex(), 0, fonts.size() - 1);
    fonts.clear();
    endRemoveRows();
}


