#ifndef FONTLISTMODEL_H
#define FONTLISTMODEL_H

#include <QAbstractListModel>
#include <font.h>
#include <QList>
#include <QDebug>
#include <QStyledItemDelegate>


class FontListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit FontListModel(QObject *parent = nullptr);
    FontListModel(const QList<Font> fontList, QObject *parent = nullptr) : QAbstractListModel(parent), fontList(fontList) {}

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void insertRow(int row, Font font);


private:
    QList<Font> fontList;
};

#endif // FONTLISTMODEL_H
