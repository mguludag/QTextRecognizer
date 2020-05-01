#ifndef ROTATEWIDGET_HPP
#define ROTATEWIDGET_HPP

#include <QDial>
#include <QWidget>

namespace Ui {
class RotateWidget;
}

class RotateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RotateWidget(QWidget *parent = nullptr);
    ~RotateWidget();
    QDial *getDial();

private slots:
    void on_dial_valueChanged(int value);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::RotateWidget *ui;
    int angle = 0;
};

#endif // ROTATEWIDGET_HPP
