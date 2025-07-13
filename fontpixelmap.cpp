#include "fontpixelmap.h"
#include <QFontMetrics>

FontPixelMap::FontPixelMap() {}

int FontPixelMap::getWidth() const
{
    QFontMetrics fontMetrics(font);
    return fontMetrics.horizontalAdvance(character);
}

int FontPixelMap::getHeight() const
{
    QFontMetrics fontMetrics(font);
    return fontMetrics.height();
}

QString FontPixelMap::getCharacter() const
{
    return character;
}

QVector<QVector<char> > FontPixelMap::getPreviewArray() const
{
    return previewArray;
}

QStringList FontPixelMap::getPattern() const
{
    return pattern;
}

QBitmap FontPixelMap::getFontBitmap() const
{
    return fontBitmap;
}

int FontPixelMap::getX() const
{
    return x;
}

QFont FontPixelMap::getFont() const
{
    return font;
}

int FontPixelMap::getY() const
{
    return y;
}

int FontPixelMap::getAscent() const {
    return ascent;
}

int FontPixelMap::getDescent() const {
    return descent;
}

int FontPixelMap::getFontSizeInPixel() const
{
    return font.pixelSize();
}

int FontPixelMap::getSpace() const
{
    return space;
}

QString FontPixelMap::getFontName() const
{
    return font.family().toLower();
}


