#include "fontpixelmap.h"

FontPixelMap::FontPixelMap() {}

int FontPixelMap::getWidth() const
{
    return width;
}

int FontPixelMap::getHeight() const
{
    return height;
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


