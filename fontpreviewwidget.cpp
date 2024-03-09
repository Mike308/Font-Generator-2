#include "fontpreviewwidget.h"
#include "ui_fontpreviewwidget.h"

FontPreviewWidget::FontPreviewWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FontPreviewWidget)
{
    ui->setupUi(this);
    QPalette pallete;
    pallete.setColor(QPalette::Window, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pallete);
    // update();
}

FontPreviewWidget::~FontPreviewWidget()
{
    delete ui;
}


void FontPreviewWidget::paintEvent(QPaintEvent *event) {
    QRect rect = event->rect();
    int ax, ay, aw, ah;
    rect.getRect(&ax, &ay, &aw, &ah);
    // qDebug() << "Paint event" << x << "|" << y << "|" << rect.x() << "|" << ax << "|" << "|" << aw << "|" << ah;
    rect.setX(ax + x);
    rect.setY(ay + y);
    QFont font;
    font.setFamily("Arial");
    font.setPixelSize(80);
    QFontMetrics fontMetrics(font);

    QImage image(40, 40, QImage::Format_Mono);
    image.fill(0);
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setFont(font);
    painter.drawText(rect,  "A");

}


void FontPreviewWidget::onPositionUpdate(int x, int y) {
    this->x = x;
    this->y = y;
    update();
}
