/*****************************************************************************
 * adjustwidget.cpp
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
#include "adjustwidget.hpp"
#include "ui_adjustwidget.h"

adjustWidget::adjustWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::adjustWidget)
{
    ui->setupUi(this);
    ui->horizontalSlider_Br->setValue(0);
    ui->horizontalSlider_Th->setValue(128);
    ui->horizontalSlider_Con->setValue(10);
}

adjustWidget::~adjustWidget()
{
    delete ui;
}

QSlider *adjustWidget::getBrightness()
{
    return ui->horizontalSlider_Br;
}

QSlider *adjustWidget::getContrast()
{
    return ui->horizontalSlider_Con;
}

QSlider *adjustWidget::getThreshold()
{
    return ui->horizontalSlider_Th;
}

bool adjustWidget::getIsAdaptive()
{
    return ui->checkBox->isChecked();
}

void adjustWidget::setBrightness(int value)
{
    ui->horizontalSlider_Br->setValue(value);
}

void adjustWidget::setContrast(int value)
{
    ui->horizontalSlider_Con->setValue(value);
}

void adjustWidget::setThreshold(int value)
{
    ui->horizontalSlider_Th->setValue(value);
}

void adjustWidget::setAdaptiveThreshold(bool value)
{
    ui->checkBox->setChecked(value);
}

void adjustWidget::on_horizontalSlider_Con_valueChanged(int value)
{
    ui->doubleSpinBox->setValue(static_cast<double>(value) / 10.);
    emit ui->horizontalSlider_Con->sliderReleased();
}

void adjustWidget::on_doubleSpinBox_valueChanged(double arg1)
{
    ui->horizontalSlider_Con->setValue(static_cast<int>(arg1 * 10));
}

void adjustWidget::on_horizontalSlider_Br_valueChanged(int value)
{
    ui->spinBox_Br->setValue(value);
    emit ui->horizontalSlider_Br->sliderReleased();
}

void adjustWidget::on_horizontalSlider_Th_valueChanged(int value)
{
    ui->spinBox_Th->setValue(value);
    emit ui->horizontalSlider_Th->sliderReleased();
}

void adjustWidget::on_spinBox_Br_valueChanged(int arg1)
{
    ui->horizontalSlider_Br->setValue(arg1);
}

void adjustWidget::on_spinBox_Th_valueChanged(int arg1)
{
    ui->horizontalSlider_Th->setValue(arg1);
}

void adjustWidget::on_buttonBox_accepted()
{
    emit accepted();
    this->close();
}

void adjustWidget::on_buttonBox_rejected()
{
    emit rejected();
    this->close();
}

void adjustWidget::on_checkBox_toggled(bool checked)
{
    ui->horizontalSlider_Th->setDisabled(checked);
    ui->spinBox_Th->setDisabled(checked);
    ui->label_5->setDisabled(checked);
    if (checked)
        emit adaptiveThresOK();
    else
        emit adaptiveThresCncl();
}
