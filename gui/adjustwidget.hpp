/*****************************************************************************
 * adjustwidget.hpp
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
#ifndef ADJUSTWIDGET_HPP
#define ADJUSTWIDGET_HPP

#include <QSlider>
#include <QWidget>

namespace Ui {
class adjustWidget;
}

class adjustWidget : public QWidget
{
    Q_OBJECT

public:
    explicit adjustWidget(QWidget *parent = nullptr);
    ~adjustWidget();
    QSlider *getBrightness();
    QSlider *getContrast();
    QSlider *getThreshold();
    bool getIsAdaptive();

    void setBrightness(int value);
    void setContrast(int value);
    void setThreshold(int value);
    void setAdaptiveThreshold(bool value);

signals:
    /* Signal to be sent in the event that there was a mouse click on the object
     * */
    void accepted();
    void rejected();
    void adaptiveThresOK();
    void adaptiveThresCncl();

private slots:

    void on_horizontalSlider_Con_valueChanged(int value);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_horizontalSlider_Br_valueChanged(int value);

    void on_horizontalSlider_Th_valueChanged(int value);

    void on_spinBox_Br_valueChanged(int arg1);

    void on_spinBox_Th_valueChanged(int arg1);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_checkBox_toggled(bool checked);

private:
    Ui::adjustWidget *ui;
    enum Threshold { Adaptive, Manual };
};

#endif // ADJUSTWIDGET_HPP
