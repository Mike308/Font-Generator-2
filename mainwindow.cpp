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
    fontsListModel = new FontsListModel(fonts);
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

void MainWindow::prepareArray(FontPixelMap font)
{
    bool ok;
    ui->plainTextEdit->appendHtml("<b>int fonts[] = {</b>");
    for (int r = 0; r < font.getFontBitmap().height(); r++) {
        QString pattern;
        for (int c = 0; c < font.getFontBitmap().width(); c++) {
            pattern.append(QString(font.getPreviewArray()[r][c]));
        }
        int decimalValue = font.getPattern()[r].toInt(&ok, 2);
        QString hex = QString("0x").append(QString::number(decimalValue, 16).toUpper());
        ui->plainTextEdit->appendPlainText(hex.append(",//").append(pattern.replace("*", " * ").replace(".", " . ")));
    }
    ui->plainTextEdit->appendHtml("<b>};</b");
    fontsListModel->insertRow(fontsListModel->rowCount(), font);
    ui->fontListView->setModel(fontsListModel);
    ui->label_3->setPixmap(QPixmap::fromImage(font.getFontBitmap().toImage().scaled(80, 80)));
}

QVector <QVector <char>> MainWindow::preparePreviewArray(QBitmap fontBitmap)
{
    QVector <QVector <char>> previewArray(fontBitmap.height(), QVector <char> (fontBitmap.width(), ' '));
    QImage fontBitmapImage = fontBitmap.toImage();
    for (int r = 0; r < fontBitmap.height(); r++) {
        QString bin;
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
    QBitmap fontBitmap(8, 8);
    fontBitmap.clear();
    QFont qFont;
    qFont.setFamily(ui->fontComboBox->currentFont().family());
    qFont.setPixelSize(ui->fontSizeSpinBox->value());
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
    FontPixelMap font(fontRectangle.width(), fontRectangle.height(), c, previewArray, fontTableValues, fontBitmap, qFont, x, y);
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
    }

}


void MainWindow::on_fontListView_pressed(const QModelIndex &index)
{
    if (fontsListModel->rowCount() > 0) {
        QFont qFont;
        qFont.setFamily(ui->fontComboBox->currentFont().family());
        qFont.setPixelSize(ui->fontSizeSpinBox->value());
        emit fontUpdate(qFont, fontsListModel->getData(index.row()).getX(), fontsListModel->getData(index.row()).getY(), fontsListModel->getData(index.row()).getCharacter().at(0).toLatin1());
    }
}


void MainWindow::on_upBtn_clicked()
{

}

