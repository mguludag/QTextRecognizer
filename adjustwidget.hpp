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
