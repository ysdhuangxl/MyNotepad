#include "searchdialog.h"
#include "QMessageBox"
#include "ui_searchdialog.h"
using namespace Qt;

SearchDialog::SearchDialog(QWidget *parent, QPlainTextEdit *textEdit)
    : QDialog(parent), ui(new Ui::SearchDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    pTextEdit = textEdit;

    // 设置游标起始位置
    QTextCursor c = pTextEdit->textCursor();
    c.setPosition(0);
    pTextEdit->setTextCursor(c);

    ui->rbDown->setChecked(true);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_btCancel_clicked()
{
    accept();
}

void SearchDialog::on_btFindNext_clicked()
{
    QString target = ui->searchText->text();

    if (target == "" || pTextEdit == nullptr)
        return;

    QString text = pTextEdit->toPlainText();
    QTextCursor c = pTextEdit->textCursor();
    int index = -1;

    if (ui->rbDown->isChecked()) {
        index = text.indexOf(target,
                             c.position(),
                             ui->cbCaseSensitive->isChecked() ? CaseSensitive : CaseInsensitive);
        if (index >= 0) {
            c.setPosition(index);
            c.setPosition(index + target.length(), QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
        }
    } else if (ui->rbUp->isChecked()) {
        index = text.lastIndexOf(target,
                                 c.position() - text.length() - 1,
                                 ui->cbCaseSensitive->isChecked() ? CaseSensitive : CaseInsensitive);
        if (index >= 0) {
            c.setPosition(index + target.length());
            c.setPosition(index, QTextCursor::KeepAnchor);

            pTextEdit->setTextCursor(c);
        }
    }
    if (index < 0) {
        QMessageBox msg(this);
        msg.setWindowFlag(Qt::Drawer);
        msg.setWindowTitle("记事本");
        msg.setIcon(QMessageBox::Information);
        msg.setText(QString("找不到") + target);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }
}
