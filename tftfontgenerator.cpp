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
                            "} <b>t_font</b><br>;";
    lines.append("<br>");
    lines.append(typeDefStruct);
}

void TFTFontGenerator::prepareTypeDefStructContent(FontPixelMap fontPixelMap, QStringList &lines)
{
    QString content = "{$space, $height, $width}, //$c<br>";
    QMap<QString, QString> placeholders {{"$space", QString::number(fontPixelMap.getSpace())}, {"$height", QString::number(fontPixelMap.getHeight())}, {"$width", QString::number(fontPixelMap.getWidth())}, {"$c", fontPixelMap.getCharacter()}};
    lines.append(Utils::prepareTemplate(content, placeholders));
}

QString TFTFontGenerator::prepareCArray(FontPixelMap fontPixelMap)
{
    QStringList lines;
    QString arrayHeader = "const uint8_t $font_name[$len][$len] = {";
    lines.append(Utils::prepareTemplate(arrayHeader, {{"$font_name", fontPixelMap.getFontName().append("_").append(QString::number(fontPixelMap.getFontSizeInPixel()).append("_px"))}, {"$len", QString::number(fontPixelMap.getFontSizeInPixel())}}));
    prepareCArrayContent(fontPixelMap, lines);
    lines.append("};");
    prepareTypeDefStruct(lines);
    QString fontSpecificationHeader = "const t_font fontSpec_$font_name = ";
    lines.append(Utils::prepareTemplate(fontSpecificationHeader, {{"$font_name", fontPixelMap.getFontName().append("_").append(QString::number(fontPixelMap.getFontSizeInPixel()).append("_px"))}}));
    prepareTypeDefStructContent(fontPixelMap, lines);
    return lines.join("\r\n");
}

QString TFTFontGenerator::prepareCArray(QList<FontPixelMap> fontPixelMaps)
{
    QStringList lines;
    QString fontArrayHeader = "const uint8_t $font_name[$row][$col] = {<br>";
    lines.append(Utils::prepareTemplate(fontArrayHeader, {{"$font_name", fontPixelMaps[0].getFontName().append("_").append(QString::number(fontPixelMaps[0].getFontSizeInPixel()).append("_px"))}, {"$row", QString::number(fontPixelMaps.length() * fontPixelMaps[0].getFontSizeInPixel())}, {"$col", QString::number(fontPixelMaps[0].getFontSizeInPixel())}}));
    for (FontPixelMap fontPixelMap : fontPixelMaps) {
        prepareCArrayContent(fontPixelMap, lines);
    }
    lines.append("<b>};</b>");
    prepareTypeDefStruct(lines);
    QString arrayHeader = "const t_font font_spec_$font_name[$arr_length] = {\n";
    QMap<QString, QString> placeholders {{"$font_name", fontPixelMaps[0].getFontName().append("_").append(QString::number(fontPixelMaps[0].getFontSizeInPixel()).append("_px"))}, {"$arr_length", QString::number(fontPixelMaps.length())}};
    lines.append(Utils::prepareTemplate(arrayHeader, placeholders));
    for (FontPixelMap fontPixelMap : fontPixelMaps) {
        prepareTypeDefStructContent(fontPixelMap, lines);
    }
    lines.append("};");
    return lines.join("\r\n");
}


