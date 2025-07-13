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

void CodePreview::renderCode(QString fontDefinitionCode, QString fontTypeCode)
{
    SyntaxHighlighter *fontDefinitionCodeHighlighter = new SyntaxHighlighter(this);
    SyntaxHighlighter *fontTypeHeaderCodeHighlighter = new SyntaxHighlighter(this);
    ui->codePlainText->clear();
    fontDefinitionCodeHighlighter->setDocument(ui->codePlainText->document());
    ui->codePlainText->appendHtml(fontDefinitionCode);
    ui->codePlainText->scroll(0, 0);
    ui->fontTypeHeaderPlainText->clear();
    fontTypeHeaderCodeHighlighter->setDocument(ui->fontTypeHeaderPlainText->document());
    ui->fontTypeHeaderPlainText->appendHtml(fontTypeCode);
    ui->fontTypeHeaderPlainText->scroll(0, 0);
}

void CodePreview::on_saveBtn_clicked()
{
    save(ui->codePlainText->toPlainText());
}


void CodePreview::on_copyBtn_clicked()
{
    QClipboard * clipboard = QApplication::clipboard();
    clipboard->setText(ui->codePlainText->toPlainText(), QClipboard::Clipboard);
}



void CodePreview::on_copyBtnFontType_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->fontTypeHeaderPlainText->toPlainText());
}


void CodePreview::on_saveBtnFontType_clicked()
{
    save(ui->fontTypeHeaderPlainText->toPlainText());
}

void CodePreview::save(const QString content)
{
    QFileDialog fileDialog(this);
    fileDialog.setNameFilter(tr("Header C File (*.h)"));
    QString fileName = fileDialog.getSaveFileName(this, "Export header file", NULL, tr("*.h"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly)) {
        QTextStream headerFileStream(&file);
        headerFileStream << content;
    }

    file.close();
}

