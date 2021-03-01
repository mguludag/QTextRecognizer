#ifndef SETTINGSWIDGET_HPP
#define SETTINGSWIDGET_HPP

#include "../settings.hpp"
#include <QCloseEvent>
#include <QFileDialog>
#include <QWidget>

static Settings m_settings(Settings::regFormat, "mguludag");

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
    void onIsLight(bool b);

signals:
    void style();

private:
    Ui::SettingsWidget *ui;
    QString directory;
    int theme;
};

#endif // SETTINGSWIDGET_HPP
