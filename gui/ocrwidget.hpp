/*****************************************************************************
 * ocrwidget.hpp
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
#ifndef OCRWIDGET_HPP
#define OCRWIDGET_HPP

#include "../improcessing/preprocessimage.hpp"
#include "settings.hpp"

#include <QCloseEvent>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QListView>
#include <QMap>
#include <QPushButton>
#include <QStandardItemModel>
#include <QThread>
#include <QWidget>
#include <QMessageBox>

namespace Ui {
class OCRWidget;
}

class OCRWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OCRWidget(QWidget *parent = nullptr);
    ~OCRWidget();
    void init();
    void initPSMModel();
    void checkLangs();
    void setSelLangs(QList<QString> list);
    void setPsmMode(QString psmmode);
    void setImage(QImage img);
    void setImageList(QList<QImage> &_images);
    void startOCR();
    void getSelection();
    int getConfidence();
    QList<QImage> getImageList();

    QPushButton getOCRButton();
    QString getPsmMode();
    QList<QString> getSelLangs();

    QString getResult() const;
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);

signals:
    void ocrStarted();
    void ocrFinished();

public slots:
    void initLangModel();
    void clearResult();
    void initSelLangs();

private slots:
    void on_pushButton_clicked();

private:
    Ui::OCRWidget *ui;
    QImage *m_ocrImg = nullptr;
    QThread *thread, *thread1, *thread2;
    tesseract::TessBaseAPI *tess = nullptr;
    QStandardItemModel *m_mdllng = nullptr, *m_mdlpsm = nullptr;
    QMap<QString, QString> *m_maplng = nullptr;
    QMap<QString, tesseract::PageSegMode> *m_mappsm = nullptr;
    QList<QImage> *m_images = nullptr;
    QString blacklist, whitelist;
    QFileInfoList fileList;
    cv::Mat *m_imgMat = nullptr;
    QString result;
    QItemSelectionModel *selectionModel = nullptr;
    Settings *m_settings = nullptr;
    QString langs = "";
    QStringList selLang;
    QString directory;
    int confidence=0;
};

#endif // OCRWIDGET_HPP
