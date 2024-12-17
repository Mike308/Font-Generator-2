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
    rect.setX(ax + x);
    rect.setY(ay + y);
    qDebug() << "Preview rect x: " << rect.x() << " y: " << rect.y();
    QFontMetrics fontMetrics(font);
    QRect fontRect = fontMetrics.boundingRect(QString(this->c));
    qDebug() << "X: " << fontRect.x() << " Y: " << fontRect.y();
    if (font.pixelSize() > 0) {
         font.setPixelSize(font.pixelSize() * 10);
         QPainter painter(this);
         painter.setPen(Qt::black);
         painter.setFont(font);
         painter.drawText(rect,  QString(this->c));
    }

}


void FontPreviewWidget::onPositionUpdate(int x, int y) {
    this->x = x;
    this->y = y;
    update();
}

void FontPreviewWidget::onFontUpdate(QFont font, int x, int y, char c)
{
    this->font = font;
    this->x = x;
    this->y = y;
    this->c = c;
    update();
}
