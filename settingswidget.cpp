#include "settingswidget.hpp"
#include "ui_settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    m_settings = new Settings(Settings::regFormat, "mguludag");

    if (m_settings->readSettings("Lang", "Dir").toString().isEmpty())
        directory = QApplication::applicationDirPath() + "/tessdata/";
    else
        directory = m_settings->readSettings("Lang", "Dir").toString();

    ui->lineEdit_dir->setText(directory);
    ui->comboBox->setCurrentIndex(m_settings->loadStyle());
    m_settings->setStyle(m_settings->loadStyle());
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::on_toolButton_clicked()
{
    directory = QFileDialog::getExistingDirectory(this, "Select tessdata folder", directory);
    ui->lineEdit_dir->setText(directory);
    m_settings->writeSettings("Lang", "Dir", directory);
}

void SettingsWidget::on_comboBox_currentIndexChanged(int index)
{
    m_settings->setStyle(index);
    if (index == 3)
        isdark = true;
    else
        isdark = false;
    emit style();
    m_settings->writeSettings("Style", "Theme", index);
}
