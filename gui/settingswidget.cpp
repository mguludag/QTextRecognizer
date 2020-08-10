/*****************************************************************************
 * settingswidget.cpp
 *
 * Created: 5/28/2020 2020 by mguludag
 *
 * Copyright 2020 mguludag. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *****************************************************************************/
#include "settingswidget.hpp"
#include "ui_settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    m_settings = new Settings(Settings::regFormat, "mguludag");
    ui->setupUi(this);

    if (m_settings->readSettings("Lang", "Dir").toString().isEmpty())
        directory = QApplication::applicationDirPath() + "/tessdata/";
    else
        directory = m_settings->readSettings("Lang", "Dir").toString();

    ui->lineEdit_dir->setText(directory);
    theme = m_settings->loadStyle();
    ui->comboBox->setCurrentIndex(theme);
    m_settings->setStyle(theme);
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::closeEvent(QCloseEvent *event)
{
    event->ignore();
    m_settings->writeSettings("Style", "Theme", theme);
    event->accept();
}

void SettingsWidget::on_toolButton_clicked()
{
    directory = QFileDialog::getExistingDirectory(this, "Select tessdata folder", directory);
    if (!directory.isEmpty()) {
        ui->lineEdit_dir->setText(directory);
        m_settings->writeSettings("Lang", "Dir", directory);
    }
}

void SettingsWidget::on_comboBox_currentIndexChanged(int index)
{
    theme = index;
    m_settings->setStyle(index);
    (index == 3) ? isdark = true : isdark = false;
    emit style();
}
