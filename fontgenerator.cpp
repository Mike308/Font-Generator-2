#include "fontgenerator.h"
#include <QDebug>

FontGenerator::FontGenerator() {}

FontPixelMap FontGenerator::generateSpecificChar(QString c, int x, int y, QFont font, QSize bitmapSize)
{

    QBitmap fontBitmap(bitmapSize);
    fontBitmap.clear();

    QFontMetrics fontMetrics(font, &fontBitmap);
    QRect fontRectangle = fontMetrics.boundingRect(c.at(0));
    qDebug () << "Font rectangle size: " << fontRectangle.size();
    QPainter painter(&fontBitmap);
    painter.setFont(font);
    painter.drawText(x, y, c);
    painter.end();

    QVector <QVector <char>> binaryMapArray = FontGenerator::convertToBinaryMapArray(fontBitmap);
    QStringList fontTableValues = FontGenerator::convertToBinaryMap(fontBitmap);
    FontPixelMap fontPixelMap(fontRectangle.width(), fontRectangle.height(), c, binaryMapArray, fontTableValues, fontBitmap, font, x, y, fontMetrics.ascent(), fontMetrics.descent(), fontMetrics.horizontalAdvance(c.at(0)));
    return fontPixelMap;
}

QStringList FontGenerator::convertToBinaryMap(QBitmap fontBitmap)
{
    QStringList fontTableValues;
    QImage fontBitmapImage = fontBitmap.toImage();
    for (int r = 0; r < fontBitmap.height(); r++) {
        QString bin;
        for (int c = 0; c < fontBitmap.width(); c++) {
            bin.append((fontBitmapImage.pixelColor(c, r) == Qt::black ? "1" : "0"));
        }
        fontTableValues.append(bin);
    }
    return fontTableValues;
}

QVector<QVector<char>> FontGenerator::convertToBinaryMapArray(QBitmap fontBitmap)
{
    QVector <QVector <char>> binaryMapVector(fontBitmap.height(), QVector <char> (fontBitmap.width(), ' '));
    QImage fontBitmapImage = fontBitmap.toImage();
    for (int r = 0; r < fontBitmap.height(); r++) {
        for (int c = 0; c < fontBitmap.width(); c++) {
            binaryMapVector[r][c] = (fontBitmapImage.pixelColor(c, r) == Qt::black ? '*' : '.');
        }
    }
    return binaryMapVector;
}



