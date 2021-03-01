#include "settingswidget.hpp"
#include "ui_settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);

    if (m_settings.readSettings("Lang", "Dir").toString().isEmpty())
        directory = QApplication::applicationDirPath() + "/tessdata/";
    else
        directory = m_settings.readSettings("Lang", "Dir").toString();

    ui->lineEdit_dir->setText(directory);
    theme = m_settings.loadStyle();
    ui->comboBox->setCurrentIndex(theme);
    m_settings.setStyle(theme);
    connect(&m_settings, SIGNAL(isLight(bool)), this, SLOT(onIsLight(bool)));
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::closeEvent(QCloseEvent *event)
{
    event->ignore();
    m_settings.writeSettings("Style", "Theme", theme);
    event->accept();
}

void SettingsWidget::on_toolButton_clicked()
{
    directory = QFileDialog::getExistingDirectory(this, "Select tessdata folder", directory);
    if (!directory.isEmpty()) {
        directory.replace('/','\\');
        ui->lineEdit_dir->setText(directory);
        m_settings.writeSettings("Lang", "Dir", directory);
    }
}

void SettingsWidget::on_comboBox_currentIndexChanged(int index)
{
    theme = index;
    m_settings.setStyle(index);
    (index == 3) ? isdark = true : isdark = false;
    emit style();
}

void SettingsWidget::onIsLight(bool b)
{
    isdark=!b;
    emit style();
}
