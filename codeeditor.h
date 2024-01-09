#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QObject>
#include <QPlainTextEdit>

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

#include "syntaxhighlighter.h"

#include <QMouseEvent>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void setSyntaxHighlighter(SyntaxHighlighter *highlighter);



protected:
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;


signals:
    void linkActivated(const QUrl &url);

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

public slots:
    void showLineNumberArea(bool flag);

private:
    QWidget *lineNumberArea;
    SyntaxHighlighter *syntaxHighlighter;


};
class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor) {}

    QSize sizeHint() const override { return QSize(codeEditor->lineNumberAreaWidth(), 0); }

protected:
    void paintEvent(QPaintEvent *event) override { codeEditor->lineNumberAreaPaintEvent(event); }

private:
    CodeEditor *codeEditor;
};

#endif // CODEEDITOR_H
