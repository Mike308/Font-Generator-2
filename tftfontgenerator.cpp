#include "tftfontgenerator.h"
#include "utils.h"

TFTFontGenerator::TFTFontGenerator() {}

FontPixelMap TFTFontGenerator::generateSpecificCharForTFT(QString c, int x, int y, QFont font, QSize bitmapSize)
{
    return generateSpecificChar(c, x, y, font, bitmapSize);
}

QStringList TFTFontGenerator::prepareCArrayContent(FontPixelMap fontPixelMap, QStringList &lines)
{
    for (int r = 0; r < fontPixelMap.getFontBitmap().height(); r++) {
        QString pattern;
        for (int c = 0; c < fontPixelMap.getFontBitmap().width(); c++) {
            pattern.append(QString(fontPixelMap.getPreviewArray()[r][c]));
        }
        QStringList hexValues = Utils::binStringToHexSeparated(fontPixelMap.getPattern().at(r));
        lines.append(QString("<p style=\"text-indent: 30px;\">{").append(hexValues.join(',').append("},</p>")));
    }
    return lines;
}

QString TFTFontGenerator::prepareCArray(FontPixelMap fontPixelMap)
{
    QStringList lines;
    lines.append("<b>int fonts[][] = {</b>");
    (prepareCArrayContent(fontPixelMap, lines));
    lines.append("<b>};</b");
    return lines.join("\r\n");
}

QString TFTFontGenerator::prepareCArray(QList<FontPixelMap> fontPixealMaps)
{
    QStringList lines;
    lines.append("<b>int fonts[][] = {</b>");
    for (FontPixelMap fontPixelMap : fontPixealMaps) {
        prepareCArrayContent(fontPixelMap, lines);
    }
    lines.append("<b>};</b>");
    return lines.join("\r\n");
}


