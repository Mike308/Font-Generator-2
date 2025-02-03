#ifndef FONTGENERATOR_H
#define FONTGENERATOR_H

#include <QBitmap>
#include <QFont>
#include <QRect>
#include <QPainter>
#include <fontpixelmap.h>

class FontGenerator
{
public:
    FontGenerator();
    virtual QString prepareCArray(FontPixelMap fontPixelMap) = 0;
    virtual QString prepareCArray(QList<FontPixelMap> fontPixealMaps) = 0;

protected:
    FontPixelMap generateSpecificChar(QString c, int x, int y, QFont font, QSize bitmapSize);
    QStringList convertToBinaryMap(QBitmap fontBitmap);
    QVector <QVector <char>> convertToBinaryMapArray(QBitmap fontBitmap);

};

#endif // FONTGENERATOR_H
