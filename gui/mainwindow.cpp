#include "mainwindow.hpp"

#define POPUP_Y 60
#define ROTATE_POPUP_X 20
#define ADJ_POPUP_X 70
#define OCR_POPUP_X 160
#define SETT_POPUP_X -380

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init();
    initDockTBar();
    initMainTbar();
    initPoints();
    initDrawTools();
    setIcons();

    connect(ui->graphicsView, SIGNAL(releaseSignal()), this, SLOT(hidePopups()));
    connect(m_scene, SIGNAL(drop()), this, SLOT(getFilename()));
    connect(m_settwidget, SIGNAL(style()), this, SLOT(setIcons()));
    connect(ui->graphicsView, SIGNAL(pressSignal()), this, SLOT(beginRect()));
    connect(ui->graphicsView, SIGNAL(releaseSignal()), this, SLOT(endRect()));
    connect(ui->graphicsView, SIGNAL(moveSignal()), this, SLOT(writePos()));
    connect(ui->actionAccept, SIGNAL(triggered()), this, SLOT(applyPreprocess()));
    connect(ui->actionCancel, SIGNAL(triggered()), this, SLOT(cancelPreprocess()));
    connect(m_rwidget, SIGNAL(angleChanged()), this, SLOT(rotatePixmap()));
    connect(m_adjWidget->getBrightness(), SIGNAL(sliderReleased()), this, SLOT(setBrightness()));
    connect(m_adjWidget->getContrast(), SIGNAL(sliderReleased()), this, SLOT(setContrast()));
    connect(m_adjWidget, SIGNAL(accepted()), this, SLOT(acceptAdjust()));
    connect(m_adjWidget, SIGNAL(rejected()), this, SLOT(rejectAdjust()));
    connect(m_adjWidget->getThreshold(), SIGNAL(sliderReleased()), this, SLOT(setMThreshold()));
    connect(m_adjWidget, SIGNAL(adaptiveThresOK()), this, SLOT(setAThreshold()));
    connect(m_adjWidget, SIGNAL(adaptiveThresCncl()), this, SLOT(setAThreshold()));
    connect(m_ocrWidget, SIGNAL(ocrFinished()), this, SLOT(showResult()));
    connect(m_ocrWidget, SIGNAL(ocrStarted()), this, SLOT(onProgress()));
}

MainWindow::~MainWindow()
{
    delete m_DockTBar;
    delete m_rwidget;
    delete z;
    delete m_settings;
    delete m_pixmap;
    delete m_scene;
    delete m_imgQ;
    delete m_imgMat;
    delete m_imgMatTh;
    delete m_imgQ_copy;
    delete m_imgMat_orig;
    delete m_ocrWidget;
    delete ui;
}

void MainWindow::init()
{
    z = new Graphics_view_zoom(ui->graphicsView);
    z->set_modifiers(Qt::ControlModifier);
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    m_settings = new Settings(Settings::regFormat, "mguludag");
    //restoreState(m_settings->readSettings("Window", "State").toByteArray());
    restoreGeometry(m_settings->readSettings("Window", "Geometry").toByteArray());

    m_imgQ = new QImage;
    m_imgQ_copy = new QImage;
    m_imgMat = new cv::Mat;
    m_imgMat_orig = new cv::Mat;
    m_imgMatTh = new cv::Mat;
    m_etimer = new QElapsedTimer;
    m_images = new QList<QImage>;

    m_menu = new QMenu;
    m_menu->addAction(ui->actionAccept);
    m_menu->addAction(ui->actionCancel);
    ui->actionPreprocess->setMenu(m_menu);

    m_menu_clearlayout = new QMenu;
    m_menu_clearlayout->addAction(ui->actionClear_layout);
    ui->actionAuto_detect_layout->setMenu(m_menu_clearlayout);

    m_rwidget = new RotateWidget;
    m_rwidget->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    m_rwidget->setWindowOpacity(0.9);
    m_rwidget->hide();

    m_adjWidget = new adjustWidget;
    m_adjWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    m_adjWidget->setWindowOpacity(0.9);
    m_adjWidget->hide();

    m_ocrWidget = new OCRWidget;
    m_ocrWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    m_ocrWidget->setWindowOpacity(0.9);
    m_ocrWidget->hide();

    m_settwidget = new SettingsWidget;
    m_settwidget->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    m_settwidget->setWindowOpacity(0.9);
    m_settwidget->hide();

    m_pixmap = new QPixmap;
    t_pixmap = new QPixmap;
    m_pixmapItem = new MyPixmapItem;
    m_scene = new MyScene;
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setMouseTracking(true);

    m_pixmapItem->setTransformationMode(Qt::SmoothTransformation);
}

void MainWindow::initMainTbar()
{
    QWidget *empty = new QWidget();
    empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->mainToolBar->addWidget(empty);
    ui->mainToolBar->addAction(ui->actionSettings);
    ui->mainToolBar->addAction(ui->actionAbout);
    enableDisableActions(ui->actionOpenImage, false);
    ui->actionOpenImage->setEnabled(true);
}

void MainWindow::initDockTBar()
{
    m_DockTBar = new QToolBar();
    ui->gridLayout->addWidget(m_DockTBar);
    ui->gridLayout->addWidget(ui->plainTextEdit);
    m_DockTBar->addAction(ui->actionCopy_to_clipboard);
    m_DockTBar->addAction(ui->actionClear_text);
    m_DockTBar->addAction(ui->actionSave);
    m_DockTBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void MainWindow::initPoints()
{
    m_pts[0] = new MyPoint(m_scene);
    m_pts[1] = new MyPoint(m_scene);
    m_pts[2] = new MyPoint(m_scene);
    m_pts[3] = new MyPoint(m_scene);

    connect(m_pts[0], SIGNAL(signal1()), this, SLOT(getPoints()));
    connect(m_pts[1], SIGNAL(signal1()), this, SLOT(getPoints()));
    connect(m_pts[2], SIGNAL(signal1()), this, SLOT(getPoints()));
    connect(m_pts[3], SIGNAL(signal1()), this, SLOT(getPoints()));

    m_polyf = new MyPolyF(m_scene);
}

void MainWindow::initDrawTools()
{
    m_pen.setColor(Qt::blue);
    m_pen1.setColor(Qt::white);
    m_brush.setStyle(Qt::BrushStyle::NoBrush);
    m_brush.setColor(Qt::red);

    ui->statusBar->addWidget(ui->label);

    ui->statusBar->addPermanentWidget(ui->label_2);

    m_pen.setWidth(4);
    m_pen1.setWidth(6);
}

//QT_EVENTS_BEGIN
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Alt:
        ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
        break;
    case Qt::Key_Escape:
        cancelPreprocess();
        if(onprogress){
            m_ocrWidget->close();
            showResult();
        }
        break;
    case Qt::Key_Shift:
        ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
        pressShift = true;
        break;
    case Qt::Key_Delete:
        removeItems(*m_scene, "rect");
        rects_size = 0;
        break;
    default:
        key = 0;
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Alt:
        ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
        break;
    case Qt::Key_Shift:
        pressShift = false;
        break;
    default:
        break;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    hidePopups();
}

void MainWindow::mousePressEvent(QMouseEvent *) {}

void MainWindow::showEvent(QShowEvent *)
{
    ui->graphicsView->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::mouseMoveEvent(QMouseEvent *) {}

void MainWindow::resizeEvent(QResizeEvent *)
{
    ui->graphicsView->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_rwidget->close();
    m_adjWidget->close();
    m_ocrWidget->close();
    m_settwidget->close();
    m_settings->writeSettings("Window", "Geometry", saveGeometry());
    //m_settings->writeSettings("Window", "State", saveState());
    event->accept();
}

void MainWindow::moveEvent(QMoveEvent *)
{
    m_rwidget->move(this->geometry().x() + ROTATE_POPUP_X, this->geometry().y() + POPUP_Y);
    m_adjWidget->move(this->geometry().x() + ADJ_POPUP_X, this->geometry().y() + POPUP_Y);
    m_ocrWidget->move(this->geometry().x() + OCR_POPUP_X, this->geometry().y() + POPUP_Y);
    m_settwidget->move(this->geometry().right() + SETT_POPUP_X, this->geometry().y() + POPUP_Y);
}

void MainWindow::hidePopups()
{
    if (m_rwidget->isVisible())
        ui->actionRotate->toggle();
    else if (m_adjWidget->isVisible())
        ui->actionAdjust_image->toggle();
    else if (m_ocrWidget->isVisible())
        ui->actionRecognize_text->toggle();
}
//QT_EVENTS_END

void MainWindow::enableDisableActions(QAction *except, bool enabled)
{
    for (auto &i : ui->mainToolBar->actions()) {
        if (i != except)
            i->setEnabled(enabled);
    }
    ui->actionAbout->setEnabled(true);
    ui->actionSettings->setEnabled(true);
}

void MainWindow::openImage()
{
    if (filename.length() > 0) {
        m_pixmap->load(filename);
        m_pixmapItem->setPixmap(*m_pixmap);
        m_pixmapItem->setTransformOriginPoint(m_pixmapItem->boundingRect().center());
        ui->graphicsView->rotate(*m_pixmapItem, *m_scene, 0);
        if (m_scene->items().size() > 0) {
            removeItems(*m_scene, "point");
            removeItems(*m_scene, "poly");
            removeItems(*m_scene, "rect");
            m_scene->removeItem(m_pixmapItem);
        }
        b_cont = c_cont = at_cont = mt_cont = true;
        rects_size = 0;
        m_rwidget->getDSpinBox()->setValue(0);
        m_scene->addItem(m_pixmapItem);
        m_scene->items().first()->setData(0, "image0");
        centerImage();
        if (m_pixmap->width() > 0)
            enableDisableActions(ui->actionOpenImage, true);
    }
}

void MainWindow::getFilename()
{
    filename = m_scene->filename;
    openImage();
}

void MainWindow::showPreprocess()
{
    previewpreproc = true;
    ui->actionPreprocess->setToolTip(
        "You can drag-drop the corners and/or click this again to apply "
        "transformation or press ESC to cancel");
    ui->label->setText(
        "You can drag-drop the corners and apply transformation or press ESC to cancel");
    enableDisableActions(ui->actionPreprocess, false);
    removeItems(*m_scene, "poly");
    removeItems(*m_scene, "point");
    *m_imgQ = getImage(*m_scene, m_pixmapItem->sceneBoundingRect());
    if (m_pointQ.size() > 0) {
        m_pointQ.clear();
        removeItems(*m_scene, "point");
        removeItems(*m_scene, "poly");
    }
    if (m_pointQ.size() < 3) {
        *m_imgMat = ConvertImage::qimageToMatRef(*m_imgQ, CV_8UC4);
        m_pointStd = PreprocessImage::getPoints(*m_imgMat);

        if (m_pointStd.size() > 3) {
            for (auto i : m_pointStd) {
                m_pointQ.push_back(
                    QPointF(static_cast<double>(i.x) / 1.92, static_cast<double>(i.y) / 1.92));
            }

            drawPolygon();
            checkPoints();
            drawPolygon();
            addItems();
        }
    }
    ui->graphicsView->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::applyPreprocess()
{
    previewpreproc = false;
    ui->actionPreprocess->setChecked(false);
    ui->actionPreprocess->setToolTip("Preprocess the image for remove warp etc.");
    ui->label->setText("Ready");
    if (!cancelpreprocess) {
        removeItems(*m_scene, "poly");
        removeItems(*m_scene, "point");
        *m_imgQ = getImage(*m_scene, m_pixmapItem->sceneBoundingRect());

        m_pointStd.clear();

        for (auto i : m_pointQ) {
            m_pointStd.push_back(cv::Point2f(static_cast<float>(i.x()), static_cast<float>(i.y())));
        }
        PreprocessImage::setPoints(m_pointStd);
        if (m_imgQ->width() > 10) {
            removeItems(*m_scene, "point");
            removeItems(*m_scene, "poly");
            m_scene->removeItem(m_pixmapItem);
            *m_imgMat = ConvertImage::qimageToMatRef(*m_imgQ, CV_8UC4);
            *m_imgMat = PreprocessImage::deskewRotate(*m_imgMat);
            *m_imgQ = ConvertImage::matToQimage(*m_imgMat, QImage::Format_ARGB32);
            *m_pixmap = QPixmap::fromImage(*m_imgQ);
            m_pixmapItem->setPixmap(*m_pixmap);
            m_pixmapItem->setTransformOriginPoint(m_pixmapItem->boundingRect().center());
            m_scene->addItem(m_pixmapItem);
            centerImage();
        }
        enableDisableActions(ui->actionPreprocess, true);
    }
    ui->graphicsView->rotate(*m_pixmapItem, *m_scene, 0);
    ui->graphicsView->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

// This func taken from gImageReader source and I modified a little
void MainWindow::autoDetectLayout()
{
    removeItems(*m_scene, "rect");
    double avgDeskew = 0.0;
    int nDeskew = 0;
    QList<QRectF> rects;
    rects_size = 0;
    QImage img = getImage(*m_scene, m_pixmapItem->sceneBoundingRect());
    tesseract::TessBaseAPI tess;
    tess.InitForAnalysePage();
    tess.SetPageSegMode(tesseract::PSM_AUTO_ONLY);
    tess.SetImage(img.bits(), img.width(), img.height(), 4, img.bytesPerLine());
    tesseract::PageIterator *it = tess.AnalyseLayout();
    if (it && !it->Empty(tesseract::RIL_BLOCK)) {
        do {
            int x1, y1, x2, y2;
            tesseract::Orientation orient;
            tesseract::WritingDirection wdir;
            tesseract::TextlineOrder tlo;
            float deskew;
            it->BoundingBox(tesseract::RIL_BLOCK, &x1, &y1, &x2, &y2);
            it->Orientation(&orient, &wdir, &tlo, &deskew);
            avgDeskew += static_cast<double>(deskew);
            ++nDeskew;
            float width = 2 + x2 - x1, height = 2 + y2 - y1;
            if (width > 10 && height > 10) {
                rects.append(
                    QRectF(x1 - 1, y1 - 1, static_cast<double>(width), static_cast<double>(height)));
            }
        } while (it->Next(tesseract::RIL_BLOCK));
    }
    delete it;

    avgDeskew = qRound(((avgDeskew / nDeskew) / CV_PI * 180.0) * 10.0) / 10.0;
    if (std::abs(avgDeskew) > 0.1) {
        double newangle = m_rwidget->getDSpinBox()->value() - avgDeskew;
        m_rwidget->getDSpinBox()->setValue(newangle);
    } else {
        for (int i = rects.size(); i-- > 1;) {
            for (int j = i; j-- > 0;) {
                if (rects[j].intersects(rects[i])) {
                    rects[j] = rects[j].united(rects[i]);
                    rects.removeAt(i);
                    break;
                }
            }
        }
        for (int i = 0, n = rects.size(); i < n; ++i) {
            m_rect = new MyRectF(m_scene,
                                 rects[i].topLeft(),
                                 rects[i].bottomRight(),
                                 ++rects_size,
                                 m_pixmapItem->sceneBoundingRect());
            m_scene->addItem(m_rect);
            m_scene->items().first()->setData(0, "rect");
            m_scene->items().first()->setData(1, m_rect->getRect());
            m_scene->items().first()->setData(2, rects_size);
        }
    }
    tess.Clear();
    tess.End();
}

void MainWindow::drawPolygon()
{
    m_polyf->setBrect(m_pixmapItem->sceneBoundingRect());
    m_polyf->setPoly(m_pointQ);

    m_pts[0]->setRatio(static_cast<int>(m_pixmapItem->pixmap().rect().height() * 0.03));
    m_pts[1]->setRatio(static_cast<int>(m_pixmapItem->pixmap().rect().height() * 0.03));
    m_pts[2]->setRatio(static_cast<int>(m_pixmapItem->pixmap().rect().height() * 0.03));
    m_pts[3]->setRatio(static_cast<int>(m_pixmapItem->pixmap().rect().height() * 0.03));

    m_pts[0]->setPos(m_pointQ[0]);
    m_pts[1]->setPos(m_pointQ[1]);
    m_pts[2]->setPos(m_pointQ[2]);
    m_pts[3]->setPos(m_pointQ[3]);

    m_polyf->setPoly(m_pointQ);

    m_scene->update();
}

void MainWindow::checkPoints()
{
    if (!m_pixmapItem->contains(m_pointQ.at(0)))
        m_pointQ[0] = m_pixmapItem->sceneBoundingRect().bottomLeft();
    if (!m_pixmapItem->contains(m_pointQ.at(1)))
        m_pointQ[1] = m_pixmapItem->sceneBoundingRect().topLeft();
    if (!m_pixmapItem->contains(m_pointQ.at(2)))
        m_pointQ[2] = m_pixmapItem->sceneBoundingRect().topRight();
    if (!m_pixmapItem->contains(m_pointQ.at(3)))
        m_pointQ[3] = m_pixmapItem->sceneBoundingRect().bottomRight();

    if (m_polyf->shape().length() < m_pixmapItem->pixmap().rect().height() * 0.05) {
        m_pointQ[0] = m_pixmapItem->sceneBoundingRect().bottomLeft();
        m_pointQ[1] = m_pixmapItem->sceneBoundingRect().topLeft();
        m_pointQ[2] = m_pixmapItem->sceneBoundingRect().topRight();
        m_pointQ[3] = m_pixmapItem->sceneBoundingRect().bottomRight();
        removeItems(*m_scene, "point");
        removeItems(*m_scene, "poly");
    }
}

void MainWindow::removeItems(QGraphicsScene &scene, QString name)
{
    for (auto &i : scene.items()) {
        if (i->data(0).toString() == name)
            scene.removeItem(i);
    }
}

void MainWindow::addItems()
{
    m_scene->addItem(m_polyf);
    m_scene->items().first()->setData(0, "poly");

    for (auto &i : m_pts) {
        m_scene->addItem(i);
        m_scene->items().first()->setData(0, "point");
    }
}

QImage MainWindow::getImage(QGraphicsScene &scene, const QRectF &_rect)
{
    QImage image(_rect.size().toSize(), QImage::Format_RGBA8888);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    for (auto &var : scene.items()) {
        if (var != m_pixmapItem)
            var->hide();
    }
    scene.render(&painter, image.rect(), _rect);
    for (auto &var : scene.items()) {
        if (var != m_pixmapItem)
            var->show();
    }
    return image;
}

void MainWindow::getPoints()
{
    m_pointQ[0] = m_pts[0]->pos();
    m_pointQ[1] = m_pts[1]->pos();
    m_pointQ[2] = m_pts[2]->pos();
    m_pointQ[3] = m_pts[3]->pos();

    drawPolygon();
}

void MainWindow::rotatePixmap()
{
    m_angle = m_rwidget->getDSpinBox()->value();
    ui->graphicsView->rotate(*m_pixmapItem, *m_scene, m_angle);
    //m_scene->update();
}

void MainWindow::centerImage()
{
    ui->graphicsView->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->centerOn(ui->graphicsView->rect().center());
}

void MainWindow::cancelPreprocess()
{
    previewpreproc = false;
    removeItems(*m_scene, "point");
    removeItems(*m_scene, "poly");
    cancelpreprocess = true;
    ui->actionPreprocess->setChecked(false);
    enableDisableActions(ui->actionPreprocess, true);
    m_pixmapItem->setPixmap(QPixmap::fromImage(*m_imgQ));
    cancelpreprocess = false;
    m_scene->update();
}

void MainWindow::setBrightness()
{
    int value = m_adjWidget->getBrightness()->value();
    if (b_cont)
        *m_imgMat = PreprocessImage::adjustBrightness(*m_imgMat_orig, value);
    else
        *m_imgMat = PreprocessImage::adjustBrightness(*m_imgMat, value);
    c_cont = false;
    at_cont = false;
    mt_cont = false;
    *m_imgQ_copy = ConvertImage::matToQimageRef(*m_imgMat, QImage::Format_RGBA8888);
    *m_pixmap = QPixmap::fromImage(*m_imgQ_copy);
    m_pixmapItem->setPixmap(*m_pixmap);
    m_scene->update();
    *m_imgMatTh = *m_imgMat;
}

void MainWindow::setContrast()
{
    double value = m_adjWidget->getContrast()->value() / 10;
    if (c_cont)
        *m_imgMat = PreprocessImage::adjustContrast(*m_imgMat_orig, value);
    else
        *m_imgMat = PreprocessImage::adjustContrast(*m_imgMat, value);
    b_cont = false;
    at_cont = false;
    mt_cont = false;
    *m_imgQ_copy = ConvertImage::matToQimageRef(*m_imgMat, QImage::Format_RGBA8888);
    *m_pixmap = QPixmap::fromImage(*m_imgQ_copy);
    m_pixmapItem->setPixmap(*m_pixmap);
    m_scene->update();
    *m_imgMatTh = *m_imgMat;
}

void MainWindow::setAThreshold()
{
    if (m_adjWidget->getIsAdaptive()) {
        PreprocessImage::adaptThreshold(*m_imgMat, false, 3, 1);
    } else
        *m_imgMat = *m_imgMatTh;
    b_cont = false;
    c_cont = false;
    mt_cont = false;
    *m_imgQ_copy = ConvertImage::matToQimageRef(*m_imgMat, QImage::Format_RGBA8888);
    *m_pixmap = QPixmap::fromImage(*m_imgQ_copy);
    m_pixmapItem->setPixmap(*m_pixmap);
    m_scene->update();
}

void MainWindow::setMThreshold()
{
    int value = m_adjWidget->getThreshold()->value();
    if (mt_cont)
        *m_imgMat = PreprocessImage::manualThreshold(*m_imgMat_orig, value, 1);
    else
        *m_imgMat = PreprocessImage::manualThreshold(*m_imgMat, value, 1);
    b_cont = false;
    c_cont = false;
    at_cont = false;
    *m_imgQ_copy = ConvertImage::matToQimageRef(*m_imgMat, QImage::Format_RGBA8888);
    *m_pixmap = QPixmap::fromImage(*m_imgQ_copy);
    m_pixmapItem->setPixmap(*m_pixmap);
    m_scene->update();
}

void MainWindow::acceptAdjust()
{
    if (m_imgQ_copy->width() > 1) {
        *m_imgQ = *m_imgQ_copy;
        *m_pixmap = QPixmap::fromImage(*m_imgQ);
        m_pixmapItem->setPixmap(*m_pixmap);
    }

    m_scene->update();
    ui->actionAdjust_image->setChecked(false);
    enableDisableActions(ui->actionAdjust_image, true);
    b_cont = true;
    c_cont = true;
    at_cont = true;
    mt_cont = true;
}

void MainWindow::rejectAdjust()
{
    *m_pixmap = QPixmap::fromImage(*m_imgQ);
    m_pixmapItem->setPixmap(*m_pixmap);
    m_scene->update();
    ui->actionAdjust_image->setChecked(false);
    enableDisableActions(ui->actionAdjust_image, true);
    b_cont = true;
    c_cont = true;
    at_cont = true;
    mt_cont = true;
}

void MainWindow::showResult()
{
    m_ocrWidget->close();
    QApplication::restoreOverrideCursor();
    onprogress=false;
    ui->label->setText("OCR finished in "+QString::number(m_etimer->elapsed()) + " msecs | Avg. word confidence is "+QString::number(m_ocrWidget->getConfidence())+"%\t");
    ui->actionRecognize_text->setChecked(false);
    ui->plainTextEdit->setPlainText(m_ocrWidget->getResult());
}

void MainWindow::onProgress()
{
    m_etimer->start();
    onprogress=true;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->label->setText("Please wait OCR in progress... To cancel press ESC.");
}

void MainWindow::writePos()
{
    ui->label_2->setText("(" + QString::number(ui->graphicsView->getProps().m_pos.x()) + ", "
                         + QString::number(ui->graphicsView->getProps().m_pos.y()) + ")");
}

void MainWindow::beginRect()
{
    if (ui->graphicsView->dragMode() == QGraphicsView::RubberBandDrag && !previewpreproc) {
        if (pressShift) {
            m_rect
                = new MyRectF(m_scene,
                              ui->graphicsView->mapToScene(ui->graphicsView->getProps().m_topleft),
                              ui->graphicsView->mapToScene(ui->graphicsView->getProps().m_topleft),
                              ++rects_size,
                              m_pixmapItem->sceneBoundingRect());
        } else {
            rects_size = 0;
            removeItems(*m_scene, "rect");

            m_rect
                = new MyRectF(m_scene,
                              ui->graphicsView->mapToScene(ui->graphicsView->getProps().m_topleft),
                              ui->graphicsView->mapToScene(ui->graphicsView->getProps().m_topleft),
                              ++rects_size,
                              m_pixmapItem->sceneBoundingRect());
        }
    }
}

void MainWindow::endRect()
{
    if (ui->graphicsView->dragMode() == QGraphicsView::RubberBandDrag && !previewpreproc
        && m_scene->items().size() > 0) {
        m_rect->setBottomRight(
            ui->graphicsView->mapToScene(ui->graphicsView->getProps().m_bottomright));
        if (ui->graphicsView->getProps().m_topleft != ui->graphicsView->getProps().m_bottomright) {
            m_scene->addItem(m_rect);
            m_scene->items().first()->setData(0, "rect");
            m_scene->items().first()->setData(1, m_rect->getRect());
            m_scene->items().first()->setData(2, rects_size);
            ui->label->setText(
                "You are selecting the area for recognize. Press SHIFT for multiselect "
                "or If you want clear areas press DELETE.");
        } else {
            --rects_size;
            for (auto &var : m_scene->items()) {
                if (var->data(0) == "rect") {
                    rects_size = var->data(2).toInt();
                    break;
                }
            }
            if (rects_size < 0)
                rects_size = 0;
        }
    }
}

QPointF MainWindow::getRotatedPoint(QPointF p, QPointF center, qreal angleRads)
{
    qreal x = p.x();
    qreal y = p.y();

    double s = std::sin(angleRads);
    double c = std::cos(angleRads);

    // translate point back to origin:
    x -= center.x();
    y -= center.y();

    // rotate point
    double xnew = x * c - y * s;
    double ynew = x * s + y * c;

    // translate point back:
    x = xnew + center.x();
    y = ynew + center.y();

    return QPointF(x, y);
}

QRectF MainWindow::getMinimumBoundingRect(QRectF r, qreal angleRads)
{
    QPointF topLeft = getRotatedPoint(r.topLeft(), r.center(), angleRads);
    QPointF bottomRight = getRotatedPoint(r.bottomRight(), r.center(), angleRads);
    QPointF topRight = getRotatedPoint(r.topRight(), r.center(), angleRads);
    QPointF bottomLeft = getRotatedPoint(r.bottomLeft(), r.center(), angleRads);

    // getMin and getMax just return the min / max of their arguments

    qreal minX = qMin(topLeft.x(), qMin(bottomRight.x(), qMin(topRight.x(), bottomLeft.x())));
    qreal minY = qMin(topLeft.y(), qMin(bottomRight.y(), qMin(topRight.y(), bottomLeft.y())));

    qreal maxX = qMax(topLeft.x(), qMax(bottomRight.x(), qMax(topRight.x(), bottomLeft.x())));
    qreal maxY = qMax(topLeft.y(), qMax(bottomRight.y(), qMax(topRight.y(), bottomLeft.y())));

    return QRectF(QPointF(minX, minY), QPointF(maxX, maxY));
}

qreal MainWindow::degToRad(qreal deg)
{
    deg = deg < 0.0 ? deg + 360.0 : deg >= 360.0 ? deg - 360.0 : deg;
    return deg * PI / 180;
}

QRectF MainWindow::getRotatedRect(QRectF _rect)
{
    QRectF rotatedRect(getRotatedPoint(_rect.topLeft(), _rect.center(), degToRad(m_angle)),
                       _rect.size());
    return rotatedRect;
}

// ACTIONS BEGIN
void MainWindow::on_actionOpenImage_triggered()
{
    filename = QFileDialog::getOpenFileName(this,
                                            "Please Select The Image",
                                            "",
                                            tr("Image Files (*.png *.jpg *.bmp)"));
    openImage();
}

void MainWindow::on_actionPreprocess_triggered()
{
    if (!previewpreproc)
        showPreprocess();
    else
        ui->actionPreprocess->menu()->popup(QCursor::pos());
}

void MainWindow::on_actionRotate_toggled(bool arg1)
{
    m_rwidget->move(this->geometry().x() + ROTATE_POPUP_X, this->geometry().y() + POPUP_Y);

    m_polyf->setTransformOriginPoint(m_pixmapItem->boundingRect().center());

    if (arg1) {
        m_rwidget->show();
        enableDisableActions(ui->actionRotate, false);
    } else {
        m_rwidget->hide();
        enableDisableActions(ui->actionRotate, true);
    }
}

void MainWindow::on_actionBest_Fit_triggered()
{
    centerImage();
}

void MainWindow::on_actionZoom_In_triggered()
{
    z->gentle_zoom(1.10);
}

void MainWindow::on_actionZoom_Out_triggered()
{
    z->gentle_zoom(0.90);
}

void MainWindow::on_actionRecognize_text_toggled(bool arg1)
{
    removeItems(*m_scene, "point");
    removeItems(*m_scene, "poly");
    m_images->clear();

    m_ocrWidget->move(this->geometry().x() + OCR_POPUP_X, this->geometry().y() + POPUP_Y);

    if (rects_size < 1)
        m_images->prepend((getImage(*m_scene, m_pixmapItem->sceneBoundingRect())));
    else {
        for (auto &var : m_scene->items()) {
            if (var->data(0).toString() == "rect")
                m_images->prepend(getImage(*m_scene, var->data(1).toRectF()));
        }
    }

    m_ocrWidget->setImageList(*m_images);

    if (arg1) {
        m_ocrWidget->show();
        enableDisableActions(ui->actionRecognize_text, !arg1);
    } else {
        enableDisableActions(ui->actionRecognize_text, !arg1);
        m_ocrWidget->hide();
    }
}

void MainWindow::on_actionAdjust_image_toggled(bool checked)
{
    removeItems(*m_scene, "poly");
    removeItems(*m_scene, "point");
    *m_imgQ = getImage(*m_scene, m_pixmapItem->sceneBoundingRect());
    //m_imgQ->save("123.png");
    *m_imgMat_orig = ConvertImage::qimageToMatRef(*m_imgQ, CV_8UC4);
    *m_imgMat = *m_imgMatTh = *m_imgMat_orig;

    m_adjWidget->move(this->geometry().x() + ADJ_POPUP_X, this->geometry().y() + POPUP_Y);

    if (checked) {
        enableDisableActions(ui->actionAdjust_image, !checked);
        m_adjWidget->show();
    } else {
        m_adjWidget->hide();
        enableDisableActions(ui->actionAdjust_image, !checked);
    }
}

void MainWindow::on_actionCopy_to_clipboard_triggered()
{
    m_clipboard->setText(ui->plainTextEdit->toPlainText(), QClipboard::Clipboard);
}

void MainWindow::on_actionClear_text_triggered()
{
    ui->plainTextEdit->clear();
    m_ocrWidget->clearResult();
}

void MainWindow::on_actionSave_triggered()
{
    QString nameFile = QFileDialog::getSaveFileName(this,
                                                    tr("Save text to file"),
                                                    "",
                                                    tr("Text Document (*.txt);"));
    if (nameFile != "") {
        QFile file(nameFile);

        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << ui->plainTextEdit->toPlainText();
            file.flush();
            file.close();
        } else {
            QMessageBox::critical(this, tr("Error"), tr("Error while saving file"));
            return;
        }
    }
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dialog;
    dialog.exec();
}

void MainWindow::on_actionSettings_toggled(bool arg1)
{
    m_settwidget->move(this->geometry().right() + SETT_POPUP_X, this->geometry().y() + POPUP_Y);

    if (arg1)
        m_settwidget->show();
    else
        m_settwidget->hide();
}

void MainWindow::on_actionAuto_detect_layout_triggered()
{
    if (!autolayout && rects_size < 1)
        autoDetectLayout();
    else
        ui->actionAuto_detect_layout->menu()->popup(QCursor::pos());
}

void MainWindow::on_actionClear_layout_triggered()
{
    removeItems(*m_scene, "rect");
    rects_size = 0;
}
