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

protected:
    void paintEvent(QPaintEvent *event) override;

public slots:
    void onPositionUpdate(int x, int y);

};

#endif // FONTPREVIEWWIDGET_H
