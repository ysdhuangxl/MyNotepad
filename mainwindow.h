#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();

    void on_actionFind_triggered();

    void on_actionReplace_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_textEdit_textChanged();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_textEdit_undoAvailable(bool b);

    void on_textEdit_copyAvailable(bool b);

    void on_textEdit_redoAvailable(bool b);

    void on_actionFontColor_triggered();

    void on_actionEditorBackgroundColor_triggered();

    void on_actionFontBackgroundColor_triggered();

    void on_actionLineWrap_triggered();

    void on_actionFont_triggered();

    void on_actionShowStatusBar_triggered();

    void on_actionShowToolBar_triggered();

    void on_actionSelectAll_triggered();

    void on_actionExit_triggered();

    void on_textEdit_cursorPositionChanged();

    void on_actionShowLineNumber_triggered(bool checked);

private:
    Ui::MainWindow *ui;

    QLabel statusCursorLabel;
    QLabel statusLabel;
    QString filePath;
    bool textChanged;

    void openFile(const QString &filename);

    bool userEditConfirmed();
};
#endif // MAINWINDOW_H
