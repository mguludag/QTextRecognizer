#include "rotatewidget.hpp"
#include "ui_rotatewidget.h"

RotateWidget::RotateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RotateWidget)
{
    ui->setupUi(this);
}

RotateWidget::~RotateWidget()
{
    delete ui;
}

QDoubleSpinBox *RotateWidget::getDSpinBox()
{
    return ui->doubleSpinBox;
}

void RotateWidget::on_dial_valueChanged(int value)
{
    ui->doubleSpinBox->setValue(value);
}

void RotateWidget::on_doubleSpinBox_valueChanged(double arg1)
{
    ui->dial->setValue(static_cast<int>(arg1));
    emit angleChanged();
}
