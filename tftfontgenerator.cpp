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

void TFTFontGenerator::prepareTypeDefStruct(QStringList &lines)
{
    QString typeDefStruct = "<b>typedef struct </b> {\n"
                            "    uint8_t space;\n"
                            "    uint8_t height;\n"
                            "    uint8_t width;\n"
                            "} <b>t_font</b>;";
    lines.append(typeDefStruct);
}

void TFTFontGenerator::prepareTypeDefStructContent(FontPixelMap fontPixelMap, QStringList &lines)
{
    QString content = "{$space, $height, $width},<br>";
    QMap<QString, QString> placeholders {{"$space", QString::number(fontPixelMap.getWidth())}, {"$height", QString::number(fontPixelMap.getHeight())}, {"$width", QString::number(fontPixelMap.getWidth())}};
    lines.append(Utils::prepareTemplate(content, placeholders));
}

QString TFTFontGenerator::prepareCArray(FontPixelMap fontPixelMap)
{
    QStringList lines;
    lines.append("<b>int fonts[][] = {</b>");
    prepareCArrayContent(fontPixelMap, lines);
    lines.append("<b>};</b");
    prepareTypeDefStruct(lines);
    prepareTypeDefStructContent(fontPixelMap, lines);
    return lines.join("\r\n");
}

QString TFTFontGenerator::prepareCArray(QList<FontPixelMap> fontPixelMaps)
{
    QStringList lines;
    lines.append("<b>int fonts[][] = {</b>");
    for (FontPixelMap fontPixelMap : fontPixelMaps) {
        prepareCArrayContent(fontPixelMap, lines);
    }
    lines.append("<b>};</b>");
    prepareTypeDefStruct(lines);
    QString arrayHeader = "t_font fonts[$arr_length] = {";
    QMap<QString, QString> placeholders {{"$arr_length", QString::number(fontPixelMaps.length())}};
    lines.append(Utils::prepareTemplate(arrayHeader, placeholders));
    for (FontPixelMap fontPixelMap : fontPixelMaps) {
        prepareTypeDefStructContent(fontPixelMap, lines);
    }
    lines.append("}");
    return lines.join("\r\n");
}


