#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class SyntaxHighlighter : public QSyntaxHighlighter
{
public:
    explicit SyntaxHighlighter(QObject *parent = nullptr);

protected:
    virtual void highlightBlock(const QString &text) override;
};

#endif // SYNTAXHIGHLIGHTER_H
