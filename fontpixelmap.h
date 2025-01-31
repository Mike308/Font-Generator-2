#ifndef FONTPIXELMAP_H
#define FONTPIXELMAP_H

#include <QString>
#include <QBitmap>
#include <QStringList>
#include <QVector>
#include <QFont>

class FontPixelMap
{
public:
    FontPixelMap();

    FontPixelMap(int width, int height, QString character,
                 QVector<QVector<char>> previewArray, QStringList pattern,
                 QBitmap fontBitmap, QFont font, int x, int y, int ascent, int descent)
        : width(width), height(height), character(std::move(character)),
        previewArray(std::move(previewArray)), pattern(std::move(pattern)),
        fontBitmap(std::move(fontBitmap)),
        font(std::move(font)), x(std::move(x)), y(std::move(y)), ascent(std::move(ascent)), descent(std::move(descent)) {}

    int getWidth() const;

    int getHeight() const;

    QString getCharacter() const;

    QVector<QVector<char>> getPreviewArray() const;

    QStringList getPattern() const;

    QBitmap getFontBitmap() const;

    int getX() const;


    int getY() const;
    int getAscent() const;
    int getDescent() const;

private:
    int width;
    int height;
    QString character;
    QVector <QVector <char>> previewArray;
    QStringList pattern;
    QBitmap fontBitmap;
    QFont font;
    int x, y, ascent, descent;

};

#endif // FONTPIXELMAP_H
