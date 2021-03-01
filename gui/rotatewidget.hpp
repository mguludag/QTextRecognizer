#ifndef ROTATEWIDGET_HPP
#define ROTATEWIDGET_HPP

#include <QDoubleSpinBox>
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
    QDoubleSpinBox *getDSpinBox();

signals:
    void angleChanged();

private slots:
    void on_dial_valueChanged(int value);

    void on_doubleSpinBox_valueChanged(double arg1);

private:
    Ui::RotateWidget *ui;
    int angle = 0;
};

#endif // ROTATEWIDGET_HPP
