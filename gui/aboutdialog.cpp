#include "aboutdialog.hpp"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_label_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link, QUrl::TolerantMode));
    if (link == "qt")
        QApplication::aboutQt();
}
