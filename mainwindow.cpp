#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
    fontsListModel = new FontsListModel(fonts, this);
    connect(fontsListModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(onFontListModelUpdate(QModelIndex,QModelIndex)));
}

MainWindow::~MainWindow()
{
    delete ui;
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
    if (!ui->isRescaleCheckBox->isChecked()) {
        TFTFontGenerator generator;
        ui->plainTextEdit->appendHtml(generator.prepareCArray(font));
    } else {
        LedMatrixFontGenerator generator;
        ui->plainTextEdit->appendHtml(generator.prepareCArray(font));
    }

    fontsListModel->insertRow(fontsListModel->rowCount(), font);
    ui->fontListView->setModel(fontsListModel);
    ui->label_3->setPixmap(QPixmap::fromImage(font.getFontBitmap().toImage().scaled(80, 80)));

}

void MainWindow::prepareArray(QList<FontPixelMap> fonts)
{
    if (ui->isRescaleCheckBox->isChecked()) {
        LedMatrixFontGenerator generator;
        ui->plainTextEdit->appendHtml(generator.prepareCArray(fonts));
    } else {
        TFTFontGenerator generator;
        ui->plainTextEdit->appendHtml(generator.prepareCArray(fonts));
    }

}


FontPixelMap MainWindow::generateSpecificChar(QString c, int x, int y)
{
    FontPixelMap font;

    int w = 0;
    int h = 0;
    QFont qFont;
    qFont.setFamily(ui->fontComboBox->currentFont().family());
    qFont.setPixelSize(ui->fontSizeSpinBox->value());


    if (ui->isRescaleCheckBox->isChecked()) {
        LedMatrixFontGenerator generator;
        generator.generateSpecificCharForLedMatrix(c, x, y, qFont);
    } else {
        TFTFontGenerator generator;
        font = generator.generateSpecificCharForTFT(c, x, y, qFont, QSize(ui->fontSizeSpinBox->value(), ui->fontSizeSpinBox->value()));
    }

    QBitmap fontBitmap = font.getFontBitmap();
    QFontMetrics fontMetrics(qFont, &fontBitmap);
    QRect fontRectangle = fontMetrics.boundingRect(c.at(0));
    QRect fontRect = fontMetrics.boundingRect(c);
    displayFontParamters(fontRect, fontMetrics);
    return font;
}
\

void MainWindow::on_generateBtn_clicked()
{
    if (ui->specificCharacterRBtn->isChecked()) {
        FontPixelMap font = generateSpecificChar(ui->charLineEdit->text(), ui->xEdit->text().toInt(), ui->yEdit->text().toInt());
        prepareArray(font);
    } else {
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

