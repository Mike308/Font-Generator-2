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
        lines.append(QString("<p>{").append(hexValues.join(',').append("},</p>")));
    }
    return lines;
}

void TFTFontGenerator::prepareTypeDefStruct(QStringList &lines)
{
    QString typeDefStruct = "<b>typedef struct </b> {\n"
                            "    uint8_t space;\n"
                            "    uint8_t height;\n"
                            "    uint8_t width;\n"
                            "    uint8_t font_size;"
                            "} <b>t_font</b><br>;";
    lines.append("<br>");
    lines.append(typeDefStruct);
}

void TFTFontGenerator::prepareTypeDefStructContent(FontPixelMap fontPixelMap, QStringList &lines)
{
    QString content = "{$space, $height, $width, $font_size}, //$c<br>";
    QMap<QString, QString> placeholders {{"$space", QString::number(fontPixelMap.getSpace())}, {"$height", QString::number(fontPixelMap.getHeight())}, {"$width", QString::number(fontPixelMap.getWidth())}, {"$font_size", QString::number(fontPixelMap.getFontSizeInPixel())}, {"$c", fontPixelMap.getCharacter()}};
    lines.append(Utils::prepareTemplate(content, placeholders));
}

QString TFTFontGenerator::prepareCArray(FontPixelMap fontPixelMap)
{
    QStringList lines;
    QString arrayHeader = "static const uint8_t fonts[$len][$len] = {";
    lines.append(Utils::prepareTemplate(arrayHeader, {{"$len", QString::number(fontPixelMap.getFontSizeInPixel())}}));
    prepareCArrayContent(fontPixelMap, lines);
    lines.append("};");
    prepareTypeDefStruct(lines);
    lines.append("t_font fontSpec = ");
    prepareTypeDefStructContent(fontPixelMap, lines);
    return lines.join("\r\n");
}

QString TFTFontGenerator::prepareCArray(QList<FontPixelMap> fontPixelMaps)
{
    QStringList lines;
    QString fontArrayHeader = "uint8_t fonts[$row][$col] = {<br>";
    lines.append(Utils::prepareTemplate(fontArrayHeader, {{"$row", QString::number(fontPixelMaps.length() * fontPixelMaps[0].getFontSizeInPixel())}, {"$col", QString::number(fontPixelMaps[0].getFontSizeInPixel())}}));
    for (FontPixelMap fontPixelMap : fontPixelMaps) {
        prepareCArrayContent(fontPixelMap, lines);
    }
    lines.append("<b>};</b>");
    prepareTypeDefStruct(lines);
    QString arrayHeader = "t_font fontsSpec[$arr_length] = {\n";
    QMap<QString, QString> placeholders {{"$arr_length", QString::number(fontPixelMaps.length())}};
    lines.append(Utils::prepareTemplate(arrayHeader, placeholders));
    for (FontPixelMap fontPixelMap : fontPixelMaps) {
        prepareTypeDefStructContent(fontPixelMap, lines);
    }
    lines.append("};");
    return lines.join("\r\n");
}


