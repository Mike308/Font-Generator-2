#ifndef TFTFONTGENERATOR_H
#define TFTFONTGENERATOR_H

#include "fontgenerator.h"

class TFTFontGenerator : public FontGenerator
{
public:
    TFTFontGenerator();
    FontPixelMap generateSpecificCharForTFT(QString c, int x, int y, QFont font, QSize bitmapSize);



    // FontGenerator interface
public:
    QString prepareCArray(FontPixelMap fontPixelMap) override;
    QString prepareCArray(QList<FontPixelMap> fontPixelMaps) override;

private:
    QStringList prepareCArrayContent(FontPixelMap fontPixelMap, QStringList &lines);
    void prepareTypeDefStruct(QStringList &lines);
    void prepareTypeDefStructContent(FontPixelMap fontPixelMap, QStringList &lines);

};

#endif // TFTFONTGENERATOR_H
