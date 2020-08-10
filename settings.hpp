/*****************************************************************************
 * settings.hpp
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
#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include <QApplication>
#include <QStyleFactory>
#include <QStyle>
#include <QSettings>
#include <QDesktopWidget>
#include <QToolTip>

class Settings
{
    uint8_t style = 2;
    QSettings *s = nullptr;

public:
    enum Theme { vista, classic, lightFusion, darkFusion };
    enum Format { regFormat, iniFormat };

    Settings(bool format, QString name)
    {
        if (format) {
            s = new QSettings(name, QSettings::IniFormat);
        } else {
            s = new QSettings(name, QApplication::applicationName());
        }
    }

    int loadStyle()
    {
        int val;
        s->beginGroup("Style");
        val = s->value("Theme", style).toInt(); //default theme is standard system theme
        s->endGroup();
        return val;
    }

    void setStyle(uint8_t val)
    {
        style = val;
        switch (val) {
        case vista:
        {
            qApp->setStyle(QStyleFactory::create("windowsvista"));
            qApp->setPalette(qApp->style()->standardPalette());
            break;
        }
        case classic:
        {
            qApp->setStyle(QStyleFactory::create("windows"));
            qApp->setPalette(qApp->style()->standardPalette());
            break;
        }
        case lightFusion:
        {
            qApp->setStyle(QStyleFactory::create("Fusion"));
            qApp->setPalette(qApp->style()->standardPalette());
            break;
        }
        case darkFusion:
        {
            qApp->setStyle(QStyleFactory::create("Fusion"));
            QPalette darkPalette;
            darkPalette.setColor(QPalette::Window, QColor(53,53,53));
            darkPalette.setColor(QPalette::WindowText, Qt::white);
            darkPalette.setColor(QPalette::Base, QColor(25,25,25));
            darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
            darkPalette.setColor(QPalette::ToolTipBase, QColor(53,53,53));
            darkPalette.setColor(QPalette::ToolTipText, Qt::white);
            darkPalette.setColor(QPalette::Text, Qt::white);
            darkPalette.setColor(QPalette::Button, QColor(53,53,53));
            darkPalette.setColor(QPalette::ButtonText, Qt::white);
            darkPalette.setColor(QPalette::BrightText, Qt::red);
            darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
            darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
            darkPalette.setColor(QPalette::HighlightedText, Qt::black);
            darkPalette.setColor(QPalette::Disabled,QPalette::Text,QColor(164,166,168));
            darkPalette.setColor(QPalette::Disabled,QPalette::WindowText,QColor(164,166,168));
            darkPalette.setColor(QPalette::Disabled,QPalette::ButtonText,QColor(164,166,168));
            darkPalette.setColor(QPalette::Disabled,QPalette::HighlightedText,QColor(164,166,168));
            darkPalette.setColor(QPalette::Disabled,QPalette::Base,QColor(68,68,68));
            darkPalette.setColor(QPalette::Disabled,QPalette::Window,QColor(68,68,68));
            darkPalette.setColor(QPalette::Disabled,QPalette::Highlight,QColor(68,68,68));
            QToolTip::setPalette(darkPalette);
            qApp->setPalette(darkPalette);
            break;
        }
        default:
            break;
        }
    }

    template<class T>
    void writeSettings(QString group, QString key, T option)
    {
        s->beginGroup(group);
        s->setValue(key, option);
        s->endGroup();
    }

    QVariant readSettings(QString group, QString key)
    {
        QVariant val;
        s->beginGroup(group);
        val = s->value(key);
        s->endGroup();
        return val;
    }
};

#endif // SETTINGS_HPP
