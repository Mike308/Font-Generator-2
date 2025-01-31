#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QStringList>
#include <QDebug>

class Utils
{
public:
    Utils();
    static QString binStringToHex(QString bin);
    static QStringList binStringToHexSeparated(QString bin);
};

#endif // UTILS_H
