#ifndef FONTPREVIEWWIDGET_H
#define FONTPREVIEWWIDGET_H

#include <QWidget>
#include <QImage>
#include <QFont>
#include <QPainter>
#include <QDebug>
#include <QRect>
#include <QPaintEvent>
#include <QPalette>



namespace Ui {
class FontPreviewWidget;
}

class FontPreviewWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FontPreviewWidget(QWidget *parent = nullptr);
    ~FontPreviewWidget();

private:
    Ui::FontPreviewWidget *ui;
    int x,y;
    QFont font;
    char c;

protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    void onPositionUpdate(int x, int y);
    void onFontUpdate(QFont font, int x, int y, char c);

};

#endif // FONTPREVIEWWIDGET_H
