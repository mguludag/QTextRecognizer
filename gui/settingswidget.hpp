/*****************************************************************************
 * settingswidget.hpp
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
#ifndef SETTINGSWIDGET_HPP
#define SETTINGSWIDGET_HPP

#include "../settings.hpp"
#include <QCloseEvent>
#include <QFileDialog>
#include <QWidget>

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    bool isdark = false;
    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget();
    void closeEvent(QCloseEvent *event);

private slots:
    void on_toolButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

signals:
    void style();

private:
    Ui::SettingsWidget *ui;
    Settings *m_settings;
    QString directory;
    int theme;
};

#endif // SETTINGSWIDGET_HPP
