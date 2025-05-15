#include "codepreview.h"
#include "ui_codepreview.h"
#include "syntaxhighlighter.h"

#include <QDebug>
#include <QFileDialog>
#include <QTextStream>
#include <QClipboard>

CodePreview::CodePreview(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::CodePreview)
{
    ui->setupUi(this);
}

CodePreview::~CodePreview()
{
    delete ui;
    qDebug () << "Delete code preview";

}

void CodePreview::renderCode(QString code)
{
    SyntaxHighlighter * syntaxHiglighter = new SyntaxHighlighter(this);
    ui->codePlainText->clear();
    syntaxHiglighter->setDocument(ui->codePlainText->document());
    ui->codePlainText->appendHtml(code);
    ui->codePlainText->scroll(0, 0);
}

void CodePreview::on_saveBtn_clicked()
{
    QFileDialog fileDialog(this);
    fileDialog.setNameFilter(tr("Header C File (*.h)"));
    QString fileName = fileDialog.getSaveFileName(this, "Export header file", NULL, tr("*.h"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream headerFileStream(&file);
        headerFileStream << ui->codePlainText->toPlainText();
    }

    file.close();
}


void CodePreview::on_copyBtn_clicked()
{
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setText(ui->codePlainText->toPlainText(), QClipboard::Clipboard);
}


