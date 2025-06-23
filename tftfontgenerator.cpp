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
        for (int c = 0; c < fontPixelMap.getWidth(); c++) {
            pattern.append(QString(fontPixelMap.getPreviewArray()[r][c]));
        }
        QStringList hexValues = Utils::binStringToHexSeparated(fontPixelMap.getPattern().at(r));

        lines.append(QString("<p>").append(hexValues.join(',').append(",</p>")));
    }
    return lines;
}



void TFTFontGenerator::prepareTypeDefStructContent(FontPixelMap fontPixelMap, QStringList &lines)
{
    QString content = "{$space, $height, $width}, //$c<br>";
    int width = fontPixelMap.getWidth();
    while (width % 8) {
        width++;
    }
    QMap<QString, QString> placeholders {{"$space", QString::number(fontPixelMap.getSpace())}, {"$height", QString::number(fontPixelMap.getHeight())}, {"$width", QString::number(width)}, {"$c", fontPixelMap.getCharacter()}};
    lines.append(Utils::prepareTemplate(content, placeholders));
}

QString TFTFontGenerator::prepareCArray(FontPixelMap fontPixelMap)
{
    const QString headerCintype = "#include &lt;cinttypes&gt;<br>";
    const QString headerFontType = "#include \"font_type.h\"<br>";
    QStringList lines;

    lines.append(headerCintype);
    lines.append(headerFontType);

    QString arrayHeader = "const uint8_t $font_name[] = {";
    lines.append(Utils::prepareTemplate(arrayHeader, {{"$font_name", fontPixelMap.getFontName().append("_").append(QString::number(fontPixelMap.getFontSizeInPixel()).append("_px"))}, {"$len", QString::number(fontPixelMap.getFontSizeInPixel())}}));
    prepareCArrayContent(fontPixelMap, lines);
    lines.append("};<br>");
    QString fontSpecificationHeader = "const t_font fontSpec_$font_name = ";
    lines.append(Utils::prepareTemplate(fontSpecificationHeader, {{"$font_name", fontPixelMap.getFontName().append("_").append(QString::number(fontPixelMap.getFontSizeInPixel()).append("_px"))}}));
    prepareTypeDefStructContent(fontPixelMap, lines);
    return lines.join("<br>");
}

QString TFTFontGenerator::prepareCArray(QList<FontPixelMap> fontPixelMaps)
{
    const QString headerCintype = "#include &lt;cinttypes&gt;<br>";
    const QString headerFontType = "#include \"font_type.h\"<br>";
    QStringList lines;

    lines.append(headerCintype);
    lines.append(headerFontType);

    QString fontArrayHeader = "const uint8_t $font_name[] = {<br>";
    QString fontName = fontPixelMaps[0].getFontName().replace(" ", "_").append("_").append(
        QString::number(fontPixelMaps[0].getFontSizeInPixel()).append("_px"));

    lines.append(Utils::prepareTemplate(
        fontArrayHeader,
        {{"$font_name", fontName},
         {"$row", QString::number(fontPixelMaps.length() *
                                  fontPixelMaps[0].getFontSizeInPixel())},
         {"$col", QString::number(fontPixelMaps[0].getFontSizeInPixel())}}));
    for (FontPixelMap fontPixelMap : fontPixelMaps) {
        prepareCArrayContent(fontPixelMap, lines);
    }

    lines.append("<b><br>};</b><br>");
    QString arrayHeader = "const t_font_spec font_spec_$font_name[$arr_length] = {\n";
    QMap<QString, QString> placeholders {{"$font_name", fontName}, {"$arr_length", QString::number(fontPixelMaps.length())}};
    lines.append(Utils::prepareTemplate(arrayHeader, placeholders));
    for (FontPixelMap fontPixelMap : fontPixelMaps) {
        prepareTypeDefStructContent(fontPixelMap, lines);
    }
    lines.append("};<br>");
    QString fontDef = "t_font_def $font_def_name_def = {\n"
                      ".font_array=$font_array,\n"
                      ".font_spec_array=font_spec_$font_array,\n"
                      "};";
    QMap<QString, QString> fontDefPlaceHolders {{"$font_def_name", fontName}, {"$font_array", fontName}, {"$font_spec", fontName}, {"$font_height", QString::number(fontPixelMaps[0].getFontSizeInPixel())}, {"$font_width", QString::number(fontPixelMaps[0].getFontSizeInPixel())}};
    lines.append(Utils::prepareTemplate(fontDef, fontDefPlaceHolders));
    return lines.join("\r\n");
}


