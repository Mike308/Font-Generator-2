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
    virtual QString prepareCArray(FontPixelMap fontPixelMap);


protected:
    FontPixelMap generateSpecificChar(QString c, int x, int y, QSize bitmapSize);
    QStringList convertToBinaryMap(FontPixelMap fontPixelMap);
    QVector <QVector <char>> convertToBinaryMapArray(FontPixelMap fontPixelMap);

};

#endif // FONTGENERATOR_H
