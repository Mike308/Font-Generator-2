#ifndef FONTSLISTMODEL_H
#define FONTSLISTMODEL_H

#include <QAbstractListModel>
#include <fontpixelmap.h>

class FontsListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit FontsListModel(QList<FontPixelMap> fonts, QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayPropertyRole) const override;

    FontPixelMap getData(int index) const;

    void insertRow(int row, FontPixelMap font);

private:
    QList<FontPixelMap> fonts;
};

#endif // FONTSLISTMODEL_H
