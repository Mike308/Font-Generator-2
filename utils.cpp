#include "utils.h"

Utils::Utils() {}

QString Utils::binStringToHex(QString bin)
{
    QString _bin = bin;
    QString hex;


    while ((_bin.length() % 4)) {
        _bin.prepend('0');
    }


    for (int i = 0; i < _bin.length(); i+=4) {
        QString nibble = _bin.mid(i, 4);
        bool ok;
        int val = nibble.toInt(&ok, 2);
        hex = hex.append(QString::number(val, 16)).toUpper();
    }

    return hex.prepend("0x");
}

QStringList Utils::binStringToHexSeparated(QString bin) {
    QStringList hexValues;
    QString _bin = bin;

    while ((_bin.length() % 4)) {
        _bin.prepend('0');
    }

    for (int i = 0; i < _bin.length(); i++) {
        QString nibble = _bin.mid(i, 4);
        bool ok;
        int val = nibble.toInt(&ok, 2);
        hexValues.append(QString("0x").append(QString::number(val, 16).length() == 2 ? "" : "0").append(QString::number(val, 16).toUpper()));
    }
    return hexValues;
}

QString Utils::prepareTemplate(QString line, QMap<QString, QString> map)
{
    QString _line = line;
    for (const QString &key : map.keys()) {
        _line = line.replace(key, map.value(key));
    }
    return _line;
}



