/*****************************************************************************
 * ocrwidget.cpp
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
#include "ocrwidget.hpp"
#include "ui_ocrwidget.h"

OCRWidget::OCRWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OCRWidget)
{
    ui->setupUi(this);

    init();
    initPSMModel();
}

OCRWidget::~OCRWidget()
{
    delete ui;
}

void OCRWidget::init()
{
    m_ocrImg = new QImage;
    m_images = new QList<QImage>;
    tess = new tesseract::TessBaseAPI;
    m_imgMat = new cv::Mat;
    m_settings = new Settings(Settings::regFormat, "mguludag");
    qRegisterMetaType<QItemSelection>("QSelectionModel");

    m_mdllng = new QStandardItemModel(0, 2);
    m_mdllng->setHeaderData(0, Qt::Horizontal, "Language");
    m_mdllng->setHeaderData(1, Qt::Horizontal, "Code");

    m_maplng = new QMap<QString, QString>;

    if (m_settings->readSettings("Lang", "Dir").toString().isEmpty())
        directory = QApplication::applicationDirPath() + "/tessdata/";
    else
        directory = m_settings->readSettings("Lang", "Dir").toString();

    ui->checkBox->setChecked(m_settings->readSettings("OCR", "Adaptive Threshold").toBool());
    ui->lineEdit_blacklist->setText(m_settings->readSettings("OCR", "Blacklist").toString());
    ui->lineEdit_whitelist->setText(m_settings->readSettings("OCR", "Whitelist").toString());

    thread = QThread::create([&] { checkLangs(); });
    connect(thread, SIGNAL(finished()), this, SLOT(initSelLangs()));
    thread->start();
}

void OCRWidget::initPSMModel()
{
    m_mdlpsm = new QStandardItemModel(0, 1);
    m_mappsm = new QMap<QString, tesseract::PageSegMode>;

    m_mappsm->insert("Automatic page segmentation", tesseract::PSM_AUTO);
    m_mappsm->insert("Page segmentation with orientation and script detection",
                     tesseract::PSM_AUTO_OSD);
    m_mappsm->insert("Assume single column of text", tesseract::PSM_SINGLE_COLUMN);
    m_mappsm->insert("Assume single block of vertically aligned text",
                     tesseract::PSM_SINGLE_BLOCK_VERT_TEXT);
    m_mappsm->insert("Assume a single uniform block of text", tesseract::PSM_SINGLE_BLOCK);
    m_mappsm->insert("Assume a line of text", tesseract::PSM_SINGLE_LINE);
    m_mappsm->insert("Assume a single word", tesseract::PSM_SINGLE_WORD);
    m_mappsm->insert("Assume a single word in a circle", tesseract::PSM_CIRCLE_WORD);
    m_mappsm->insert("Sparse text in no particular order", tesseract::PSM_SPARSE_TEXT);
    m_mappsm->insert("Sparse text with orientation and script detection",
                     tesseract::PSM_SPARSE_TEXT_OSD);

    for (auto &item : m_mappsm->keys()) {
        m_mdlpsm->insertRow(0);
        m_mdlpsm->setData(m_mdlpsm->index(0, 0), item);
    }

    ui->comboBox_PSM->setModel(m_mdlpsm);
    ui->comboBox_PSM->setCurrentIndex(3);
    ui->comboBox_PSM->setCurrentText(m_settings->readSettings("OCR", "PSM Mode").toString());
}

void OCRWidget::initLangModel()
{
    QString val;
    QFile file;
    file.setFileName(":/lang.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject object = document.object();

    QJsonObject::iterator i;
    for (i = object.end(); i != object.begin(); --i) {
        for (auto finfo : fileList) {
            if (finfo.fileName().remove(".traineddata") == i.key().trimmed()) {
                m_mdllng->insertRow(0);
                m_mdllng->setData(m_mdllng->index(0, 1), i.key().trimmed());
                m_mdllng->setData(m_mdllng->index(0, 0), i.value().toString().trimmed());
            }
        }
    }
    selLang = m_settings->readSettings("Lang", "Selected").toString().split('+');
}

void OCRWidget::clearResult()
{
    result.clear();
}

void OCRWidget::initSelLangs()
{
    ui->treeView->setModel(m_mdllng);
    ui->treeView->header()->resizeSection(0, 130);
    ui->treeView->header()->resizeSection(1, 60);

    for (auto &index : selLang) {
        for (auto data : m_mdllng->findItems(index, Qt::MatchExactly, 1)) {
            ui->treeView->selectionModel()->select(data->index(), QItemSelectionModel::Select);
            ui->treeView->selectionModel()->select(data->index().siblingAtColumn(0),
                                                   QItemSelectionModel::Select);
            ui->treeView->selectionModel()->select(data->index().siblingAtColumn(1),
                                                   QItemSelectionModel::Select);
        }
    }
}

void OCRWidget::checkLangs()
{
    QDir dir(directory);
    dir.setFilter(QDir::Files);
    QStringList filters;
    filters << "*.traineddata";
    dir.setNameFilters(filters);
    fileList = dir.entryInfoList();

    initLangModel();
}

void OCRWidget::setImage(QImage img)
{
    *m_ocrImg = img;
}

void OCRWidget::setImageList(QList<QImage> &_images)
{
    m_images = &_images;
}

void OCRWidget::startOCR()
{
    getSelection();
    confidence = 0;
    if (tess->Init(directory.toStdString().c_str(),
                   langs.remove(langs.size() - 1, 1).toStdString().c_str(),
                   tesseract::OEM_LSTM_ONLY)
        < 0)
        QMessageBox::warning(this,
                             "Error",
                             "Error while OCR init. Check language files and redownload them.");
    else {
        for (auto &var : *m_images) {
            tess->SetVariable("tessedit_char_whitelist",
                              ui->lineEdit_whitelist->text().toStdString().c_str());
            tess->SetVariable("tessedit_char_blacklist",
                              ui->lineEdit_blacklist->text().toStdString().c_str());
            tess->SetPageSegMode(m_mappsm->value(ui->comboBox_PSM->currentText()));
            if (ui->checkBox->isChecked()) {
                *m_imgMat = ConvertImage::qimageToMatRef(var, CV_8UC4);
                PreprocessImage::adaptThreshold(*m_imgMat, false, 3, 1);
                *m_ocrImg = ConvertImage::matToQimageRef(*m_imgMat, QImage::Format_RGBA8888);
            } else
                *m_ocrImg = var;
            tess->SetImage(m_ocrImg->bits(),
                           m_ocrImg->width(),
                           m_ocrImg->height(),
                           4,
                           m_ocrImg->bytesPerLine());
            tess->SetSourceResolution(200);

            result += QString::fromUtf8(tess->GetUTF8Text()).trimmed() + '\n';
            confidence += tess->MeanTextConf();
            tess->Clear();
        }
        confidence /= m_images->size();
        //    Pixa **ttt = nullptr;
        //    Boxa *boxes;
        //    boxes = tess->GetComponentImages(tesseract::RIL_BLOCK, true, ttt, nullptr);
        //    m_pen2.setColor(Qt::red);
        //    m_pen2.setWidth(2);
        //    m_ocrrect = new QRectF();
        //    for (int32_t i = 0; i < boxes->n; i++) {
        //        BOX *box = boxaGetBox(boxes, i, L_CLONE);
        //        m_ocrrect->setRect(box->x, box->y, box->w, box->h);
        //        m_scene->addRect(*m_ocrrect, m_pen2);
        //    }

        tess->End();
    }

    emit ocrFinished();
}

void OCRWidget::getSelection()
{
    selLang.clear();
    langs.clear();
    selectionModel = ui->treeView->selectionModel();
    QModelIndexList indexes = selectionModel->selectedIndexes();
    for (auto &index : indexes) {
        if (index.column() == 1)
            langs.append(selectionModel->model()->data(index).toString() + '+');
    }
}

int OCRWidget::getConfidence()
{
    return confidence;
}

QList<QImage> OCRWidget::getImageList()
{
    return *m_images;
}

QString OCRWidget::getResult() const
{
    return result;
}

void OCRWidget::closeEvent(QCloseEvent *event)
{
    event->ignore();
    getSelection();
    m_settings->writeSettings("Lang", "Selected", langs);
    m_settings->writeSettings("Lang", "Dir", directory);
    m_settings->writeSettings("OCR", "PSM Mode", ui->comboBox_PSM->currentText());
    m_settings->writeSettings("OCR", "Adaptive Threshold", ui->checkBox->isChecked());
    m_settings->writeSettings("OCR", "Whitelist", ui->lineEdit_whitelist->text());
    m_settings->writeSettings("OCR", "Blacklist", ui->lineEdit_blacklist->text());
    thread->quit();

    event->accept();
}

void OCRWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        thread1->terminate();
        this->close();
        emit ocrFinished();
        break;
    case Qt::Key_F5:
        thread = QThread::create([&] { checkLangs(); });
        connect(thread, SIGNAL(finished()), this, SLOT(initLangModel()));
        thread->start();
        break;
    default:
        break;
    }
}

void OCRWidget::on_pushButton_clicked()
{
    thread1 = QThread::create([&] { startOCR(); });
    thread1->start();
    emit ocrStarted();
}
