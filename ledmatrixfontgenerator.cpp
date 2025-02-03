#include "ledmatrixfontgenerator.h"
#include "utils.h"

LedMatrixFontGenerator::LedMatrixFontGenerator() {}

FontPixelMap LedMatrixFontGenerator::generateSpecificCharForLedMatrix(QString c, int x, int y, QFont font)
{
    return generateSpecificChar(c, x, y, font, QSize(8, 8));
}

void LedMatrixFontGenerator::prepareCArrayContent(QStringList &lines, FontPixelMap fontPixelMap)
{
    for (int r = 0; r < fontPixelMap.getFontBitmap().height(); r++) {
        QString pattern;
        for (int c = 0; c < fontPixelMap.getFontBitmap().width(); c++) {
            pattern.append(QString("<blockquote>").append(QString(fontPixelMap.getPreviewArray()[r][c])).append("</blockquote>"));
        }
        QString hex = Utils::binStringToHex(fontPixelMap.getPattern().at(r));
        lines.append(hex.append(",//").append(pattern.replace("*", " * ").replace(".", " . ")));
    }
}

QString LedMatrixFontGenerator::prepareCArray(FontPixelMap fontPixelMap)
{

    QStringList lines;
    lines.append("<b>int fonts[] = {</b>");
    prepareCArrayContent(lines, fontPixelMap);
    lines.append("<b>};</b");
    return lines.join("\r\n");
}

QString LedMatrixFontGenerator::prepareCArray(QList<FontPixelMap> fontPixealMaps)
{
    QStringList lines;
    lines.append("<b> int fonts[] = { </b>");
    for (FontPixelMap fontPixelMap : fontPixealMaps) {
        prepareCArrayContent(lines, fontPixelMap);
    }
    lines.append("<b>};</b>");
    return lines.join("\r\n");
}







