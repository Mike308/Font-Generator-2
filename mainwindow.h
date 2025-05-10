#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFont>
#include <QFontMetrics>
#include <QList>
#include <QDebug>
#include <QFileDialog>
#include <fontslistmodel.h>
#include <font.h>
#include <fontpixelmap.h>
#include <string>
#include <iostream>

#include <utils.h>
#include <fontgenerator.h>
#include <tftfontgenerator.h>
#include <ledmatrixfontgenerator.h>

#include "codepreview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_generateBtn_clicked();

    void on_fontListView_pressed(const QModelIndex &index);

    void on_upBtn_clicked();

    void on_fontListView_entered(const QModelIndex &index);

    void on_fontListView_activated(const QModelIndex &index);

    void on_generateArrayBtn_clicked();


    void on_updateSelectedChar_clicked();
    void on_fontSizeSpinBox_valueChanged(int arg1);

    void on_expoortBtn_clicked();

    void on_centerBtn_clicked();

    void on_leftBtn_clicked();

    void on_rightBtn_clicked();

    void on_italicBtn_toggled(bool checked);

    void on_underlineBtn_toggled(bool checked);

    void on_boldBtn_toggled(bool checked);

    void on_printableAsciiRBtn_clicked();

    void on_specificCharacterRBtn_clicked();

    void on_charLineEdit_textChanged(const QString &arg1);

    void on_downBtn_clicked();

    void on_fontListView_customContextMenuRequested(const QPoint &pos);

public slots:
    void onFontListModelUpdate(QModelIndex topLeft, QModelIndex bottomRight);
    void onInsertedRowToListModel(const QModelIndex &parent, int start, int end);
    void selectAllFonts();
    void deselectAllFonts();
    void deleteAllFonts();

signals:
    void positionUpdate(int x, int y);
    void fontUpdate(QFont font, int x, int y, char c);
    void renderCode(QString code);

private:
    Ui::MainWindow *ui;
    QList<FontPixelMap> fonts;
    FontsListModel *fontsListModel;
    void displayFontParamters(QRect fontRect, QFontMetrics fontMetrics);
    FontPixelMap generateSpecificChar(QString c, int x, int y);
    void prepareArray(FontPixelMap font);
    void prepareArray(QList<FontPixelMap> fonts);
    void refreshUI(const QModelIndex &index);
    void enabledPatternGenerateButton();
    enum Direction { UP, LEFT, RIGHT, CENTER, DOWN };
    void moveCharacter(Direction fontModification);
    bool isItalic = false;
    bool isBold = false;
    bool isUnderline = false;
    CodePreview *codePreview;

};
#endif // MAINWINDOW_H
