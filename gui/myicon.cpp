/*****************************************************************************
 * myicon.cpp
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
#include "myicon.hpp"
#include "ui_mainwindow.h"

MyIcon::MyIcon(QObject *parent)
    : QObject(parent)
    , ui(new Ui::MainWindow)
{
    
}

void MyIcon::setIcons(Ui::MainWindow *ui, bool isDark)
{
    QIcon icon;

    if (isDark) {
        icon.addFile(QString::fromUtf8(
                         ":/breeze-icons-master/icons-dark/actions/32/document-open.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionOpenImage->setIcon(icon);
        icon.addFile(QString::fromUtf8(
                         ":/breeze-icons-master/icons-dark/actions/32/trim-to-selection.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionPreprocess->setIcon(icon);
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons-dark/actions/32/edit-undo.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionRotate_Left->setIcon(icon);
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons-dark/actions/32/edit-redo.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionRotate_Right->setIcon(icon);
        icon.addFile(QString::fromUtf8(
                         ":/breeze-icons-master/icons-dark/actions/32/zoom-fit-best.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionBest_Fit->setIcon(icon);
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons-dark/actions/32/zoom-in.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionZoom_In->setIcon(icon);
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons-dark/actions/32/zoom-out.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionZoom_Out->setIcon(icon);
        icon.addFile(QString::fromUtf8(
                         ":/breeze-icons-master/icons-dark/actions/32/document-new.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionCopy_to_clipboard->setIcon(icon);
        icon.addFile(QString::fromUtf8(
                         ":/breeze-icons-master/icons-dark/actions/32/document-close.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionClear_text->setIcon(icon);
        icon.addFile(
            QString::fromUtf8(
                ":/breeze-icons-master/icons-dark/actions/32/document-edit-decrypt-verify.svg"),
            QSize(),
            QIcon::Normal,
            QIcon::Off);
        ui->actionRecognize_text->setIcon(icon);
        icon.addFile(QString::fromUtf8(
                         ":/breeze-icons-master/icons-dark/actions/32/view-refresh.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionRotate->setIcon(icon);
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons-dark/actions/32/configure.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionSettings->setIcon(icon);
        icon.addFile(QString::fromUtf8(
                         ":/breeze-icons-master/icons-dark/actions/32/help-whatsthis.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionAbout->setIcon(icon);
        icon.addFile(QString::fromUtf8(
                         ":/breeze-icons-master/icons-dark/devices/22/drive-multipartition.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionAdjust_image->setIcon(icon);
        icon.addFile(QString::fromUtf8(
                         ":/breeze-icons-master/icons-dark/actions/32/document-save.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionSave->setIcon(icon);
    } else {
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons/actions/32/document-open.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionOpenImage->setIcon(icon);
        icon.addFile(QString::fromUtf8(
                         ":/breeze-icons-master/icons/actions/32/trim-to-selection.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionPreprocess->setIcon(icon);
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons/actions/32/edit-undo.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionRotate_Left->setIcon(icon);
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons/actions/32/edit-redo.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionRotate_Right->setIcon(icon);
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons/actions/32/zoom-fit-best.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionBest_Fit->setIcon(icon);
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons/actions/32/zoom-in.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionZoom_In->setIcon(icon);
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons/actions/32/zoom-out.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionZoom_Out->setIcon(icon);
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons/actions/32/document-new.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionCopy_to_clipboard->setIcon(icon);
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons/actions/32/document-close.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionClear_text->setIcon(icon);
        icon.addFile(QString::fromUtf8(
                         ":/breeze-icons-master/icons/actions/32/document-edit-decrypt-verify.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionRecognize_text->setIcon(icon);
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons/actions/32/view-refresh.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionRotate->setIcon(icon);
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons/actions/32/configure.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionSettings->setIcon(icon);
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons/actions/32/help-whatsthis.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionAbout->setIcon(icon);
        icon.addFile(QString::fromUtf8(
                         ":/breeze-icons-master/icons/devices/22/drive-multipartition.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionAdjust_image->setIcon(icon);
        icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons/actions/32/document-save.svg"),
                     QSize(),
                     QIcon::Normal,
                     QIcon::Off);
        ui->actionSave->setIcon(icon);
    }
}
