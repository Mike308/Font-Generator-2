#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFont>
#include <QFontMetrics>
#include <QList>
#include <QDebug>
#include <fontlistmodel.h>
#include <fontslistmodel.h>
#include <font.h>
#include <fontpixelmap.h>
#include <string>
#include <iostream>



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

signals:
    void positionUpdate(int x, int y);
    void fontUpdate(QFont font, int x, int y, char c);

private:
    Ui::MainWindow *ui;
    QList<FontPixelMap> fonts;
    FontsListModel *fontsListModel;
    QByteArray convertToByteArray(const QImage &image);
    void displayFontParamters(QRect fontRect, QFontMetrics fontMetrics);
    FontPixelMap generateSpecificChar(QString c, int x, int y);
    void prepareArray(FontPixelMap font);
    QVector <QVector <char>> preparePreviewArray(QBitmap fontBitmap);
    QStringList prepareFontTableValues(QBitmap fontBitmap);
};
#endif // MAINWINDOW_H
