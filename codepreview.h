#ifndef CODEPREVIEW_H
#define CODEPREVIEW_H

#include <QFrame>

namespace Ui {
class CodePreview;
}

class CodePreview : public QFrame
{
    Q_OBJECT

public:
    explicit CodePreview(QWidget *parent = nullptr);
    ~CodePreview();

public slots:
    void renderCode(QString code);

private slots:
    void on_saveBtn_clicked();

    void on_copyBtn_clicked();

private:
    Ui::CodePreview *ui;
};

#endif // CODEPREVIEW_H
