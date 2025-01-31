#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <bitset>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this, SIGNAL(positionUpdate(int,int)), ui->widget, SLOT(onPositionUpdate(int,int)));
    connect(this, SIGNAL(fontUpdate(QFont,int,int, char)), ui->widget, SLOT(onFontUpdate(QFont,int,int, char)));
    ui->specificCharacterRBtn->setChecked(true);
    ui->fontComboBox->setCurrentText("Arial");
    ui->fontListView->setViewMode(QListView::IconMode);
    ui->fontListView->setIconSize(QSize(80, 80));
    fontsListModel = new FontsListModel(fonts);
    connect(fontsListModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(onFontListModelUpdate(QModelIndex,QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete fontsListModel;
}


void MainWindow::displayFontParamters(QRect fontRect, QFontMetrics fontMetrics)
{
    ui->hLabel->setText(QString::number(fontRect.height()));
    ui->wLabel->setText(QString::number(fontRect.width()));
    ui->ascentLabel->setText(QString::number(fontMetrics.ascent()));
    ui->descentLabel->setText(QString::number(fontMetrics.descent()));
}

void MainWindow::prepareLine(int r, QString pattern, FontPixelMap font)
{
    if (!ui->isRescaleCheckBox->isChecked()) {
        QStringList hexValues = Utils::binStringToHexSeparated(font.getPattern().at(r));
        ui->plainTextEdit->appendPlainText(QString('{').append(hexValues.join(',').append("},")));
    } else {
        QString hex = Utils::binStringToHex(font.getPattern().at(r));
        ui->plainTextEdit->appendPlainText(hex.append(",//").append(pattern.replace("*", " * ").replace(".", " . ")));
    }
}

void MainWindow::prepareArray(FontPixelMap font)
{
    bool ok;
    if (!ui->isRescaleCheckBox->isChecked())
        ui->plainTextEdit->appendHtml("<b> uint8_t fonts[][] = </b>");
    else
        ui->plainTextEdit->appendHtml("<b>int fonts[] = {</b>");
    for (int r = 0; r < font.getFontBitmap().height(); r++) {
        QString pattern;
        for (int c = 0; c < font.getFontBitmap().width(); c++) {
            pattern.append(QString(font.getPreviewArray()[r][c]));
        }
        int decimalValue = font.getPattern().at(r).toInt(&ok, 2);
        qDebug () << "Decimal value: " << decimalValue << " bin value: " <<  font.getPattern().at(r) << "new hex value: " << Utils::binStringToHex(font.getPattern().at(r));
        prepareLine(r, pattern, font);
    }
    ui->plainTextEdit->appendHtml("<b>};</b");
    fontsListModel->insertRow(fontsListModel->rowCount(), font);
    ui->fontListView->setModel(fontsListModel);
    ui->label_3->setPixmap(QPixmap::fromImage(font.getFontBitmap().toImage().scaled(80, 80)));

}

void MainWindow::prepareArray(QList<FontPixelMap> fonts)
{
    bool ok;
    ui->plainTextEdit->appendHtml("<b>uint8_t fonts[] = {</b>");
    for (FontPixelMap font : fonts) {
        for (int r = 0; r < font.getFontBitmap().height(); r++) {
            QString pattern;
            for (int c = 0; c < font.getFontBitmap().width(); c++) {
                pattern.append(QString(font.getPreviewArray()[r][c]));
            }
            int decimalValue = font.getPattern().at(r).toInt(&ok, 16);
            qDebug () << "Decimal value: " << decimalValue << " bin value: " <<  font.getPattern().at(r);
            QString hex = Utils::binStringToHex(font.getPattern().at(r));
            ui->plainTextEdit->appendPlainText(hex.append(",//").append(pattern.replace("*", " * ").replace(".", " . ")));
        }
    }
    ui->plainTextEdit->appendHtml("<b>};</b>");
    ui->plainTextEdit->appendHtml("<b> uint8_t fonts_spaceing[] = {</b>");
    for (FontPixelMap font : fonts) {
        ui->plainTextEdit->appendPlainText(QString::number(font.getWidth()).append(",//").append(font.getCharacter()));
    }
    ui->plainTextEdit->appendHtml("<b>};</b>");
}

QVector <QVector <char>> MainWindow::preparePreviewArray(QBitmap fontBitmap)
{
    qDebug () << "Fontbitmap: " << fontBitmap.height() << " " << fontBitmap.width();
    QVector <QVector <char>> previewArray(fontBitmap.height(), QVector <char> (fontBitmap.width(), ' '));
    QImage fontBitmapImage = fontBitmap.toImage();
    for (int r = 0; r < fontBitmap.height(); r++) {
        for (int c = 0; c < fontBitmap.width(); c++) {
            previewArray[r][c] = (fontBitmapImage.pixelColor(c, r) == Qt::black ? '*' : '.');
        }
    }
    return previewArray;
}

QStringList MainWindow::prepareFontTableValues(QBitmap fontBitmap)
{
    QStringList fontTableValues;
    QImage fontBitmapImage = fontBitmap.toImage();
    qDebug () << "Font bitmap: " << fontBitmap.height() << " " << fontBitmap.width();
    for (int r = 0; r < fontBitmap.height(); r++) {
        QString bin;
        for (int c = 0; c < fontBitmap.width(); c++) {
            bin.append((fontBitmapImage.pixelColor(c, r) == Qt::black ? "1" : "0"));
        }
        fontTableValues.append(bin);
    }
    return fontTableValues;
}

FontPixelMap MainWindow::generateSpecificChar(QString c, int x, int y)
{
    int w = 0;
    int h = 0;
    QFont qFont;
    qFont.setFamily(ui->fontComboBox->currentFont().family());
    qFont.setPixelSize(ui->fontSizeSpinBox->value());


    if (ui->isRescaleCheckBox->isChecked()) {
        w = 8;
        h = 8;
    } else {
        QFontMetrics fm(qFont);
        QRect fr = fm.boundingRect(c.at(0));
        w = ui->fontSizeSpinBox->value();
        h = ui->fontSizeSpinBox->value();
        qDebug () << "W: " << w << " H: " << h;
    }


    QBitmap fontBitmap(w, h);
    fontBitmap.clear();

    QFontMetrics fontMetrics(qFont, &fontBitmap);
    QRect fontRectangle = fontMetrics.boundingRect(c.at(0));
    QRect fontRect = fontMetrics.boundingRect(c);
    QPainter painter(&fontBitmap);
    painter.setFont(qFont);
    painter.drawText(x, y, c);
    painter.end();

    displayFontParamters(fontRect, fontMetrics);
    QVector <QVector <char>> previewArray = preparePreviewArray(fontBitmap);
    QStringList fontTableValues = prepareFontTableValues(fontBitmap);
    FontPixelMap font(fontRectangle.width(), fontRectangle.height(), c, previewArray, fontTableValues, fontBitmap, qFont, x, y, fontMetrics.ascent(), fontMetrics.descent());
    qDebug () << "Font spaceing: " << fontRect.width() << "for letter: " << c;
    return font;
}


QByteArray MainWindow::convertToByteArray(const QImage &image)
{
    QByteArray byteArray;
    const uchar* imageData = image.bits();
    int byteWidth = image.bytesPerLine();  // liczba bajtów na linię obrazu
    int height = image.height();

    // Kopiujemy dane pikseli do QByteArray
    for (int y = 0; y < height; ++y) {
        byteArray.append(reinterpret_cast<const char*>(imageData + y * byteWidth), byteWidth);
    }

    return byteArray;
}




void MainWindow::on_generateBtn_clicked()
{
    if (ui->specificCharacterRBtn->isChecked()) {
        FontPixelMap font = generateSpecificChar(ui->charLineEdit->text(), ui->xEdit->text().toInt(), ui->yEdit->text().toInt());
        prepareArray(font);
    } else {
        // QList<FontPixelMap> fonts;
        for (int i = 32; i < 127; i++) {
            FontPixelMap font = generateSpecificChar(QString(QChar(i)), ui->xEdit->text().toInt(), ui->yEdit->text().toInt());
            fonts.append(font);
            fontsListModel->insertRow(fontsListModel->rowCount(), font);
        }
        ui->fontListView->setModel(fontsListModel);
    }
}


void MainWindow::refreshUI(const QModelIndex &index)
{
    if (fontsListModel->rowCount() > 0) {
        FontPixelMap font = fontsListModel->getData(index.row());
        QFont qFont;
        qFont.setFamily(ui->fontComboBox->currentFont().family());
        qFont.setPixelSize(ui->fontSizeSpinBox->value());
        ui->ascentLabel->setText(QString::number(font.getAscent()));
        ui->descentLabel->setText(QString::number(font.getDescent()));
        ui->label_3->setPixmap(QPixmap::fromImage(font.getFontBitmap().toImage().scaled(80, 80)));
        emit fontUpdate(qFont, font.getX(), font.getY(), font.getCharacter().at(0).toLatin1());
    }
}

void MainWindow::on_fontListView_pressed(const QModelIndex &index)
{
    refreshUI(index);
}


void MainWindow::on_upBtn_clicked()
{

}


void MainWindow::on_fontListView_entered(const QModelIndex &index)
{
    refreshUI(index);
}


void MainWindow::on_fontListView_activated(const QModelIndex &index)
{
    refreshUI(index);
}


void MainWindow::on_generateArrayBtn_clicked()
{
    prepareArray(fontsListModel->getAllData());
}

void MainWindow::onFontListModelUpdate(QModelIndex topLeft, QModelIndex bottomRight)
{
    refreshUI(topLeft);
}


void MainWindow::on_updateSelectedChar_clicked()
{
    if (fontsListModel->rowCount() > 0) {
        QModelIndex modelIndex = ui->fontListView->currentIndex();
        FontPixelMap font = fontsListModel->getData(modelIndex.row());
        FontPixelMap updatedFont = generateSpecificChar(font.getCharacter(), ui->xEdit->text().toInt(), ui->yEdit->text().toInt());
        fontsListModel->updateRow(modelIndex.row(), updatedFont);
    }
}


void MainWindow::on_fontSizeSpinBox_valueChanged(int arg1)
{
    ui->yEdit->setText(QString::number(arg1));
}

