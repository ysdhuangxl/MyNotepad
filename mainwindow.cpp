#include "mainwindow.h"
#include "QColorDialog"
#include "QFontDialog"
#include "aboutdialog.h"
#include "qdebug.h"
#include "qfiledialog.h"
#include "qmessagebox.h"
#include "qtextstream.h"
#include "replacedialog.h"
#include "searchdialog.h"
#include "ui_mainwindow.h"

using namespace Qt;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    QDebug debug = qDebug();

    ui->setupUi(this);

    filePath = "";

    statusLabel.setMaximumWidth(200);
    statusLabel.setText("length: " + QString::number(0) + "   lines: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(200);
    statusCursorLabel.setText("Ln: " + QString::number(0) + "   Col: " + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel();
    author->setText(tr("黄小龙"));
    ui->statusbar->addPermanentWidget(author);

    textChanged = false;
    on_actionNew_triggered();

    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionPaste->setEnabled(false);

    on_actionLineWrap_triggered();

    ui->actionShowStatusBar->setChecked(true);
    ui->actionShowToolBar->setChecked(true);

    on_actionShowLineNumber_triggered(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog adlg;
    adlg.exec();
}

void MainWindow::on_actionFind_triggered()
{
    SearchDialog sdlg(this, ui->textEdit);
    sdlg.exec();
}

void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog sdlg(this, ui->textEdit);
    sdlg.exec();
}

void MainWindow::on_actionNew_triggered()
{
    if (!userEditConfirmed())
        return;

    filePath = "";
    ui->textEdit->clear();
    this->setWindowTitle(tr("新建文本文件 - 编辑器"));

    textChanged = false;
}

void MainWindow::on_actionOpen_triggered()
{
    if (!userEditConfirmed())
        return;

    QString filename = QFileDialog::getOpenFileName(this,
                                                    "打开文件",
                                                    ".",
                                                    tr("Text files(*.txt);;All(*.*)"));

    if (!filename.isEmpty()) {
        openFile(filename);
    }
}


// 新的函数，用于在新窗口中打开文件
void MainWindow::openFile(const QString &filename)
{
    QFile file(filename);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "打开文件失败");
        return;
    }

    if (filePath.isEmpty()) {
        // 如果是第一次打开文件，则在当前窗口显示
        filePath = filename;
        ui->textEdit->clear();
        this->setWindowTitle(QFileInfo(filename).absolutePath());
    } else {
        // 如果不是第一次打开文件，则在新窗口中显示
        MainWindow *newWindow = new MainWindow;
        newWindow->show();
        newWindow->openFile(filename);
        return;
    }

    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->insertPlainText(text);
    file.close();

    textChanged = false;
}




void MainWindow::on_actionSave_triggered()
{
    if (filePath == "") {
        QString filename = QFileDialog::getSaveFileName(this,
                                                        "保存文件",
                                                        ".",
                                                        tr("Text files(*.txt)"));
        QFile file(filename);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "..", "打开保存位置的文件失败");
            return;
        }
        file.close();
        filePath = filename;
    }

    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "打开文件失败");
        return;
    }

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();

    this->setWindowTitle(QFileInfo(filePath).absolutePath());

    textChanged = false;
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".", tr("Text files(*.txt)"));
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "..", "打开保存位置的文件失败");
        return;
    }
    filePath = filename;

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();

    this->setWindowTitle(QFileInfo(filePath).absolutePath());
}

bool MainWindow::userEditConfirmed()
{
    if (textChanged) {
        QString path = (filePath != "") ? filePath : "无标题.txt";

        QMessageBox msg(this);
        msg.setWindowFlag(Drawer);
        msg.setWindowTitle("...");
        msg.setIcon(QMessageBox::Question);
        msg.setText(QString("是否将更改保存到\n") + "\"" + path + "\"?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        int r = msg.exec();
        switch (r) {
        case QMessageBox::Yes:
            on_actionSave_triggered();
            break;
        case QMessageBox::No:
            textChanged = false;
            break;
        case QMessageBox::Cancel:
            return false;
        }
    }
    return true;
}

void MainWindow::on_textEdit_textChanged()
{
    if (!textChanged) {
        this->setWindowTitle("*" + this->windowTitle());
        textChanged = true;
    }
    statusLabel.setText("length: " + QString::number(ui->textEdit->toPlainText().length())
                        + "   lines: " + QString::number(ui->textEdit->document()->lineCount()));
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
    ui->actionPaste->setEnabled(true);
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
    ui->actionPaste->setEnabled(true);
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_textEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}

void MainWindow::on_textEdit_redoAvailable(bool b)
{
    ui->actionRedo->setEnabled(b);
}

void MainWindow::on_textEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);
}

void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, this);
    if (ok)
        ui->textEdit->setFont(font);
}

void MainWindow::on_actionFontColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择颜色");
    if (color.isValid()) {
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit {color:%1}").arg(color.name()));
    }
}

void MainWindow::on_actionEditorBackgroundColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择颜色");
    if (color.isValid()) {
        ui->textEdit->setStyleSheet(
            QString("QPlainTextEdit {background-color:%1}").arg(color.name()));
    }
}

void MainWindow::on_actionFontBackgroundColor_triggered()
{
    //锁定选择的文本
    QTextCursor cursor = ui->textEdit->textCursor();
    QString text = cursor.selectedText();

    //选择颜色
    QColor color = QColorDialog::getColor(Qt::black, this, "选择颜色");

    //更改文本背景颜色
    if (color.isValid()) {
        QTextCharFormat format = cursor.charFormat();
        format.setBackground(color);
        cursor.insertText(text, format);
    }
}

void MainWindow::on_actionLineWrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode = ui->textEdit->lineWrapMode();

    if (mode == QPlainTextEdit::NoWrap) {
        ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);

        ui->actionLineWrap->setChecked(true);
    } else {
        ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

        ui->actionLineWrap->setChecked(false);
    }
}

void MainWindow::on_actionShowStatusBar_triggered()
{
    bool visible = ui->statusbar->isVisible();
    ui->statusbar->setVisible(!visible);
    ui->actionShowStatusBar->setChecked(!visible);
}

void MainWindow::on_actionShowToolBar_triggered()
{
    bool visible = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionShowToolBar->setChecked(!visible);
}

void MainWindow::on_actionSelectAll_triggered()
{
    ui->textEdit->selectAll();
}

void MainWindow::on_actionExit_triggered()
{
    if (userEditConfirmed())
        exit(0);
}

void MainWindow::on_textEdit_cursorPositionChanged()
{
    int col = 0;
    int ln = 0;
    int flg = -1;
    int pos = ui->textEdit->textCursor().position();
    QString text = ui->textEdit->toPlainText();

    for (int i = 0; i < pos; i++) {
        if (text[i] == '\n') {
            ln++;
            flg = i;
        }
    }
    flg++;
    col = pos - flg;
    statusCursorLabel.setText("Ln: " + QString::number(ln + 1)
                              + "   Col: " + QString::number(col + 1));
}

void MainWindow::on_actionShowLineNumber_triggered(bool checked)
{
    ui->textEdit->showLineNumberArea(checked);
}
