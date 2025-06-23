#include "syntaxhighlighter.h"
#include <QTextFormat>
#include <QRegularExpression>
#include <QVector>
#include <QDebug>
#include <QTextBlockFormat>

SyntaxHighlighter::SyntaxHighlighter(QObject *parent)
    : QSyntaxHighlighter{parent}
{}

void SyntaxHighlighter::highlightBlock(const QString &text)
{

    QTextCursor cursor(this->document());

    struct HighlightingRule {
        QRegularExpression pattern;
        QTextCharFormat format;
        QTextBlockFormat blockFormat;
    };

    QVector<HighlightingRule> rules;

    QTextCharFormat keywordTextFormat;
    keywordTextFormat.setForeground(Qt::blue);
    keywordTextFormat.setFontWeight(QFont::Bold);


    QTextBlockFormat test;


    HighlightingRule keywordRule;
    keywordRule.pattern = QRegularExpression(R"(\b(t_font_def|t_font_spec|auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|uint8_t|uint16_t|for|goto|if|inline|int|long|register|restrict|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while|_Alignas|_Alignof|_Atomic|_Bool|_Complex|_Generic|_Imaginary|_Noreturn|_Static_assert|_Thread_local)\b)");
    keywordRule.format = keywordTextFormat;
    keywordRule.blockFormat = QTextBlockFormat();

    HighlightingRule arrayContent;
    arrayContent.pattern = QRegularExpression(R"(\{\d+\})");
    arrayContent.format = QTextCharFormat();

    QTextBlockFormat arrayContentBlockFormat;
    arrayContentBlockFormat.setLeftMargin(40);
    arrayContent.blockFormat = arrayContentBlockFormat;

    rules.append(keywordRule);
    rules.append(arrayContent);

    for (const HighlightingRule &rule : std::as_const(rules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}


