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

QDial *RotateWidget::getDial()
{
    return ui->dial;
}

void RotateWidget::on_dial_valueChanged(int value)
{
    ui->spinBox->setValue(value);
    emit ui->dial->sliderReleased();
}

void RotateWidget::on_spinBox_valueChanged(int arg1)
{
    ui->dial->setValue(arg1);
}
