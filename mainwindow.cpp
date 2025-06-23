#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QIcon>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->specificCharacterRBtn->setChecked(true);
    ui->fontComboBox->setCurrentText("Arial");
    ui->fontListView->setViewMode(QListView::IconMode);
    ui->fontListView->setIconSize(QSize(80, 80));
    ui->fontListView->setContextMenuPolicy(Qt::CustomContextMenu);
    fontsListModel = new FontsListModel(fonts, this);
    ui->generateBtn->setEnabled(false);
    ui->generateArrayBtn->setEnabled(false);
    connect(fontsListModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(onFontListModelUpdate(QModelIndex,QModelIndex)));
    connect(fontsListModel, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(onInsertedRowToListModel(QModelIndex,int,int)));
    codePreview = new CodePreview();
    connect(this, SIGNAL(renderCode(QString)), codePreview, SLOT(renderCode(QString)));
    connect(this, SIGNAL(renderCode(QString,QString)), codePreview, SLOT(renderCode(QString,QString)));
}

MainWindow::~MainWindow()
{
    delete codePreview;
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
    QString code;
    bool ok;
    if (!ui->isRescaleCheckBox->isChecked()) {
        TFTFontGenerator generator;
        code = generator.prepareCArray(font);
    } else {
        LedMatrixFontGenerator generator;
        code = generator.prepareCArray(font);
    }

    fontsListModel->insertRow(fontsListModel->rowCount(), font);
    ui->fontListView->setModel(fontsListModel);
    emit renderCode(code);
    codePreview->show();
}

void MainWindow::prepareArray(QList<FontPixelMap> fonts)
{

    QString code;
    if (ui->isRescaleCheckBox->isChecked()) {
        LedMatrixFontGenerator generator;
        code = generator.prepareCArray(fonts);
        emit renderCode(code);
    } else {
        TFTFontGenerator generator;
        code = generator.prepareCArray(fonts);
        emit renderCode(code, Utils::prepareTypeDefsStructures());
    }
    codePreview->show();

}


FontPixelMap MainWindow::generateSpecificChar(QString c, int x, int y)
{
    FontPixelMap font;

    int w = 0;
    int h = 0;
    QFont qFont;
    qFont.setFamily(ui->fontComboBox->currentFont().family());
    qFont.setPixelSize(ui->fontSizeSpinBox->value());
    qFont.setBold(isBold);
    qFont.setItalic(isItalic);
    qFont.setUnderline(isUnderline);


    if (ui->isRescaleCheckBox->isChecked()) {
        LedMatrixFontGenerator generator;
        generator.generateSpecificCharForLedMatrix(c, x, y, qFont);
    } else {
        TFTFontGenerator generator;
        QFontMetrics fontMetrics(qFont);
        qDebug () << "Font metrics: " << fontMetrics.averageCharWidth() << "|" << fontMetrics.horizontalAdvance(c);
        font = generator.generateSpecificCharForTFT(c, x, y, qFont, QSize(fontMetrics.horizontalAdvance(c, 1), fontMetrics.height()));
        for (QString line : font.getPattern()) {
            qDebug () << line;
        }
    }

    QBitmap fontBitmap = font.getFontBitmap();
    QFontMetrics fontMetrics(qFont, &fontBitmap);
    QRect fontRect = fontMetrics.boundingRect(c);
    displayFontParamters(fontRect, fontMetrics);
    return font;
}
\

void MainWindow::on_generateBtn_clicked()
{
    if (ui->specificCharacterRBtn->isChecked()) {
        FontPixelMap font = generateSpecificChar(ui->charLineEdit->text(), ui->xEdit->text().toInt(), ui->yEdit->text().toInt());
        QFontMetrics fontMetrics(font.getFont());
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
    }
}

void MainWindow::enabledPatternGenerateButton()
{
    if (ui->specificCharacterRBtn->isChecked() && !ui->charLineEdit->text().isEmpty())
        ui->generateBtn->setEnabled(true);
    else if (ui->printableAsciiRBtn->isChecked())
        ui->generateBtn->setEnabled(true);
    else ui->generateBtn->setEnabled(false);
}

void MainWindow::moveCharacter(Direction fontModification)
{
    if (fontsListModel->rowCount() > 0) {
        QItemSelectionModel *selectionModel = ui->fontListView->selectionModel();
        QModelIndexList selectedRows = selectionModel->selectedIndexes();
        int x = ui->xEdit->text().trimmed().isEmpty() ? 0 : ui->xEdit->text().toInt();
        int y = ui->yEdit->text().trimmed().isEmpty() ? 0 : ui->yEdit->text().toInt();
        int fontSize = ui->fontSizeSpinBox->value();
        for (const QModelIndex &index : selectedRows) {
            FontPixelMap font = fontsListModel->getData(index.row());
            switch (fontModification) {
                case CENTER: x = (fontSize - font.getWidth()) / 2; ui->xEdit->setText(QString::number(x)); break;
                case LEFT: x -= 1; ui->xEdit->setText(QString::number(x)); break;
                case RIGHT: x += 1; ui->xEdit->setText(QString::number(x)); break;
                case UP: y -= 1; ui->yEdit->setText(QString::number(y)); break;
                case DOWN: y +=1; ui->yEdit->setText(QString::number(y)); break;
            }

            FontPixelMap updatedFont = generateSpecificChar(font.getCharacter(), x, y);
            fontsListModel->updateRow(index.row(), updatedFont);
        }
    }
}

void MainWindow::on_fontListView_pressed(const QModelIndex &index)
{
    refreshUI(index);
}


void MainWindow::on_upBtn_clicked()
{
    moveCharacter(UP);
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

void MainWindow::onInsertedRowToListModel(const QModelIndex &parent, int start, int end)
{
    ui->generateArrayBtn->setEnabled(true);
}

void MainWindow::selectAllFonts()
{
    ui->fontListView->selectAll();
}

void MainWindow::deselectAllFonts()
{
    ui->fontListView->clearSelection();
}

void MainWindow::deleteAllFonts()
{
    fontsListModel->removeAll();
}


void MainWindow::on_updateSelectedChar_clicked()
{
    if (fontsListModel->rowCount() > 0) {
        QItemSelectionModel *selectionModel = ui->fontListView->selectionModel();
        QModelIndexList selectedRows = selectionModel->selectedIndexes();
        for (const QModelIndex &index : selectedRows) {
            FontPixelMap font = fontsListModel->getData(index.row());
            FontPixelMap updatedFont = generateSpecificChar(font.getCharacter(), ui->xEdit->text().toInt(), ui->yEdit->text().toInt());
            fontsListModel->updateRow(index.row(), updatedFont);
        }
    }
}


void MainWindow::on_fontSizeSpinBox_valueChanged(int arg1)
{
    ui->yEdit->setText(QString::number(arg1));
}


void MainWindow::on_expoortBtn_clicked()
{
    if (fontsListModel->rowCount() > 0) {
        QModelIndex modelIndex = ui->fontListView->currentIndex();
        FontPixelMap font = fontsListModel->getData(modelIndex.row());
        QFileDialog fileDialog(this);
        fileDialog.setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
        font.getFontBitmap().toImage().save(fileDialog.getSaveFileName(this, "Export font as bmp", NULL, tr("*.bmp")));
    }
}


void MainWindow::on_centerBtn_clicked()
{
    moveCharacter(CENTER);
}


void MainWindow::on_leftBtn_clicked()
{
    moveCharacter(LEFT);
}


void MainWindow::on_rightBtn_clicked()
{
    moveCharacter(RIGHT);
}


void MainWindow::on_italicBtn_toggled(bool checked)
{
    this->isItalic = checked;
}


void MainWindow::on_underlineBtn_toggled(bool checked)
{
    this->isUnderline = checked;
}


void MainWindow::on_boldBtn_toggled(bool checked)
{
    this->isBold = checked;
}


void MainWindow::on_printableAsciiRBtn_clicked()
{
    enabledPatternGenerateButton();
}


void MainWindow::on_specificCharacterRBtn_clicked()
{
    enabledPatternGenerateButton();
}


void MainWindow::on_charLineEdit_textChanged(const QString &arg1)
{
    enabledPatternGenerateButton();
}


void MainWindow::on_downBtn_clicked()
{
    moveCharacter(DOWN);
}


void MainWindow::on_fontListView_customContextMenuRequested(const QPoint &pos)
{
    QIcon selectAllIcon(":/icons/select_all.png");
    QIcon deselectIcon(":/icons/deselect.png");
    QIcon deleteAllIcon(":/icons/delete_all.png");
    QMenu contextMenu(tr("Context menu"), this);

    QAction selectAll(tr("Select all fonts"), this);
    selectAll.setIcon(selectAllIcon);

    QAction deselectAll(tr("Deselect all fonts"), this);
    deselectAll.setIcon(deselectIcon);

    QAction deleteAll(tr("Remove all fonts"), this);
    deleteAll.setIcon(deleteAllIcon);

    connect(&selectAll, SIGNAL(triggered(bool)), SLOT(selectAllFonts()));
    connect(&deselectAll, SIGNAL(triggered(bool)), SLOT(deselectAllFonts()));
    connect(&deleteAll, SIGNAL(triggered(bool)), SLOT(deleteAllFonts()));

    contextMenu.addAction(&selectAll);
    contextMenu.addAction(&deselectAll);
    contextMenu.addAction(&deleteAll);
    contextMenu.exec(ui->fontListView->mapToGlobal(pos));
}

