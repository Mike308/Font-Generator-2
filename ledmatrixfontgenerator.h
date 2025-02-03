#ifndef LEDMATRIXFONTGENERATOR_H
#define LEDMATRIXFONTGENERATOR_H

#include "fontgenerator.h"

class LedMatrixFontGenerator : public FontGenerator
{
public:
    LedMatrixFontGenerator();
    FontPixelMap generateSpecificCharForLedMatrix(QString c, int x, int y, QFont font);




    // FontGenerator interface
public:
    QString prepareCArray(FontPixelMap fontPixelMap) override;
    QString prepareCArray(QList<FontPixelMap> fontPixealMaps) override;

private:
    void prepareCArrayContent(QStringList &lines, FontPixelMap fontPixelMap);
};

#endif // LEDMATRIXFONTGENERATOR_H
