#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QDebug>

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

    connect(ui->graphicsView, SIGNAL(drop()), this, SLOT(getFilename()));
    connect(m_settwidget, SIGNAL(style()), this, SLOT(setIcons()));
    connect(ui->graphicsView, SIGNAL(pressSignal()), this, SLOT(beginRect()));
    connect(ui->graphicsView, SIGNAL(releaseSignal()), this, SLOT(endRect()));
    connect(ui->graphicsView, SIGNAL(moveSignal()), this, SLOT(writePos()));
    connect(m_rwidget->getDial(), SIGNAL(sliderReleased()), this, SLOT(rotatePixmap()));
    connect(m_adjWidget->getBrightness(), SIGNAL(sliderReleased()), this, SLOT(setBrightness()));
    connect(m_adjWidget->getContrast(), SIGNAL(sliderReleased()), this, SLOT(setContrast()));
    connect(m_adjWidget, SIGNAL(accepted()), this, SLOT(acceptAdjust()));
    connect(m_adjWidget, SIGNAL(rejected()), this, SLOT(rejectAdjust()));
    connect(m_adjWidget->getThreshold(), SIGNAL(sliderReleased()), this, SLOT(setMThreshold()));
    connect(m_adjWidget, SIGNAL(adaptiveThresOK()), this, SLOT(setAThreshold()));
    connect(m_adjWidget, SIGNAL(adaptiveThresCncl()), this, SLOT(setAThreshold()));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(blinkActionPreprocess()));
    connect(m_ocrWidget, SIGNAL(ocrFinished()), this, SLOT(showResult()));
    connect(m_ocrWidget, SIGNAL(ocrStarted()), this, SLOT(onProgress()));
}

MainWindow::~MainWindow()
{
    delete m_DockTBar;
    delete m_rwidget;
    delete z;
    delete m_settings;
    delete m_preprocImg;
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
    restoreState(m_settings->readSettings("Window", "State").toByteArray());
    restoreGeometry(m_settings->readSettings("Window", "Geometry").toByteArray());
    if (m_settings->loadStyle() == 3) {
        m_settwidget->isdark = true;
        setIcons();
    }

    m_imgQ = new QImage;
    m_imgQ_copy = new QImage;
    m_imgMat = new cv::Mat;
    m_imgMat_orig = new cv::Mat;
    m_imgMatTh = new cv::Mat;
    m_timer = new QTimer;
    m_etimer = new QElapsedTimer;
    m_images = new QList<QImage>;

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

    m_preprocImg = new PreprocessImage;
    m_pixmap = new QPixmap;
    m_pixmapItem = new QGraphicsPixmapItem;
    m_scene = new MyScene;

    //tess = new tesseract::TessBaseAPI;
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
    //connect(m_polyf,SIGNAL(signal2()),this,SLOT());
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
    case Qt::Key_1:
        key = 1;
        break;
    case Qt::Key_2:
        key = 2;
        break;
    case Qt::Key_3:
        key = 3;
        break;
    case Qt::Key_4:
        key = 4;
        break;
    default:
        key = 0;
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    // ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
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

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_rwidget->isVisible())
        ui->actionRotate->setChecked(false);
    else if (m_adjWidget->isVisible())
        ui->actionAdjust_image->setChecked(false);
    else if (m_ocrWidget->isVisible())
        ui->actionRecognize_text->setChecked(false);
    else
        m_mousePos = event->pos();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //    switch (key) {
    //    case 1:
    //        m_pts[0]->setPos(event->pos() / 1.92);
    //        m_scene->addItem(m_pts[0]);
    //        break;
    //    case 2:
    //        m_pts[1]->setPos(event->pos() / 1.92);
    //        m_scene->addItem(m_pts[1]);
    //        break;
    //    case 3:
    //        m_pts[2]->setPos(event->pos() / 1.92);
    //        m_scene->addItem(m_pts[2]);
    //        break;
    //    case 4:
    //        m_pts[3]->setPos(event->pos() / 1.92);
    //        m_scene->addItem(m_pts[3]);
    //        break;
    //    default:
    //        break;
    //    }
}

void MainWindow::showEvent(QShowEvent *)
{
    ui->graphicsView->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    //    ui->label_2->setText("(" + QString::number(event->x()) + ", " + QString::number(event->y())
    //                         + ")");
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    ui->graphicsView->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    m_rwidget->close();
    m_adjWidget->close();
    m_ocrWidget->close();
    m_settings->writeSettings("Window", "Geometry", saveGeometry());
    m_settings->writeSettings("Window", "State", saveState());
    event->accept();
}

void MainWindow::moveEvent(QMoveEvent *)
{
    m_rwidget->move(this->geometry().x() + 20, this->geometry().y() + 40);
    m_adjWidget->move(this->geometry().x() + 70, this->geometry().y() + 40);
    m_ocrWidget->move(this->geometry().x() + 90, this->geometry().y() + 40);
    m_settwidget->move(this->geometry().right() - 380, this->geometry().y() + 40);
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

void MainWindow::setIcons()
{
    QIcon icon;
    isDark = m_settwidget->isdark;
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

void MainWindow::openImage()
{
    if (filename.length() > 0) {
        m_pixmap->load(filename);
        m_pixmapItem->setPixmap(*m_pixmap);
        m_pixmapItem->setTransformOriginPoint(m_pixmapItem->boundingRect().center());
        if (m_scene->items().size() > 0) {
            removeItems(*m_scene, "point");
            removeItems(*m_scene, "poly");
            m_scene->removeItem(m_pixmapItem);
        }
        m_scene->addItem(m_pixmapItem);
        m_scene->items().first()->setData(0, "image0");
        ui->graphicsView->setScene(m_scene);
        centerImage();
        if (m_pixmap->width() > 0)
            enableDisableActions(ui->actionOpenImage, true);
    }
}

void MainWindow::getFilename()
{
    filename = ui->graphicsView->filename;
    openImage();
}

void MainWindow::showPreprocess()
{
    previewpreproc = true;
    ui->actionPreprocess->setToolTip(
        "You can drag-drop the corners and/or click this again to apply "
        "transformation or press ESC to cancel");
    ui->label->setText("You can drag-drop the corners and/or click the Preprocess button again to "
                       "apply transformation or press ESC to cancel");
    enableDisableActions(ui->actionPreprocess, false);
    m_timer->start(750);
    removeItems(*m_scene, "poly");
    removeItems(*m_scene, "point");
    *m_imgQ = getImage(*m_scene, m_pixmapItem->pixmap().rect());
    if (m_pointQ.size() > 0) {
        m_pointQ.clear();
        removeItems(*m_scene, "point");
        removeItems(*m_scene, "poly");
    }
    if (m_pointQ.size() < 3) {
        *m_imgMat = ConvertImage::qimageToMatRef(*m_imgQ, CV_8UC4);
        m_pointStd = m_preprocImg->getPoints(*m_imgMat);

        if (m_pointStd.size() > 3) {
            for (auto i : m_pointStd) {
                m_pointQ.push_back(
                    QPointF(static_cast<double>(i.x) / 1.92, static_cast<double>(i.y) / 1.92));
            }

            if (!m_pixmapItem->contains(m_pointQ.at(0)))
                m_pointQ[0] = m_pixmapItem->pixmap().rect().bottomLeft();
            if (!m_pixmapItem->contains(m_pointQ.at(1)))
                m_pointQ[1] = m_pixmapItem->pixmap().rect().topLeft();
            if (!m_pixmapItem->contains(m_pointQ.at(2)))
                m_pointQ[2] = m_pixmapItem->pixmap().rect().topRight();
            if (!m_pixmapItem->contains(m_pointQ.at(3)))
                m_pointQ[3] = m_pixmapItem->pixmap().rect().bottomRight();

            drawPolygon();
            addItems();
            if (m_polyf->shape().length() < m_pixmapItem->pixmap().rect().height() * 0.3) {
                m_pointQ[0] = m_pixmapItem->pixmap().rect().bottomLeft();
                m_pointQ[1] = m_pixmapItem->pixmap().rect().topLeft();
                m_pointQ[2] = m_pixmapItem->pixmap().rect().topRight();
                m_pointQ[3] = m_pixmapItem->pixmap().rect().bottomRight();
                removeItems(*m_scene, "point");
                removeItems(*m_scene, "poly");
                drawPolygon();
                addItems();
            }
        }
    }
    ui->graphicsView->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::applyPreprocess()
{
    previewpreproc = false;
    m_timer->stop();
    ui->actionPreprocess->setToolTip("Preprocess the image for remove warp etc.");
    ui->label->setText("Ready");
    if (!cancelpreprocess) {
        removeItems(*m_scene, "poly");
        removeItems(*m_scene, "point");
        *m_imgQ = getImage(*m_scene, m_pixmapItem->pixmap().rect());

        m_pointStd.clear();

        for (auto i : m_pointQ) {
            m_pointStd.push_back(cv::Point2f(static_cast<float>(i.x()), static_cast<float>(i.y())));
        }

        m_preprocImg->setPoints(m_pointStd);

        if (m_imgQ->width() > 10) {
            removeItems(*m_scene, "point");
            removeItems(*m_scene, "poly");
            m_scene->removeItem(m_pixmapItem);
            *m_imgMat = ConvertImage::qimageToMatRef(*m_imgQ, CV_8UC4);
            *m_imgMat = m_preprocImg->deskewRotate(*m_imgMat);
            *m_imgQ = ConvertImage::matToQimage(*m_imgMat, QImage::Format_ARGB32);
            *m_pixmap = QPixmap::fromImage(*m_imgQ);
            m_pixmapItem->setPixmap(*m_pixmap);
            m_pixmapItem->setTransformOriginPoint(m_pixmapItem->boundingRect().center());
            m_scene->addItem(m_pixmapItem);
            centerImage();
        }
        enableDisableActions(ui->actionPreprocess, true);
        //ui->actionPreprocess->setEnabled(false);
    }
}

void MainWindow::drawPolygon()
{
    m_polyf->setBrect(m_pixmapItem->pixmap().rect());
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

QImage MainWindow::getImage(QGraphicsScene &scene, const QRectF &rect)
{
    QImage image(rect.size().toSize(), QImage::Format_RGBA8888);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    //scene.render(&painter);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    QTransform t;
    //t.scale(1.005, 1.005);
    t.translate(-rect.x(), -rect.y());
    //t.translate(-0.5 * m_pixmapItem->pixmap().width(), -0.5 * m_pixmapItem->pixmap().height());
    painter.setTransform(t);
    painter.drawPixmap(0, 0, m_pixmapItem->pixmap());
    return image;
}

void MainWindow::getPoints()
{
    m_pointQ[0] = m_pts[0]->pos();
    m_pointQ[1] = m_pts[1]->pos();
    m_pointQ[2] = m_pts[2]->pos();
    m_pointQ[3] = m_pts[3]->pos();

    //    removeItems(*m_scene, "point");
    //    removeItems(*m_scene, "poly");

    drawPolygon();
}

void MainWindow::rotatePixmap()
{
    m_pixmapItem->setRotation(m_rwidget->getDial()->value());
    m_scene->update();
}

void MainWindow::centerImage()
{
    m_scene->setSceneRect(0, 0, m_pixmap->width(), m_pixmap->height());
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

void MainWindow::blinkActionPreprocess()
{
    if (state)
        ui->actionPreprocess->setIcon(stateicon1);
    else
        ui->actionPreprocess->setIcon(stateicon2);
    state = !state;
}

void MainWindow::setBrightness()
{
    int value = m_adjWidget->getBrightness()->value();
    if (b_cont)
        *m_imgMat = m_preprocImg->adjustBrightness(*m_imgMat_orig, value);
    else
        *m_imgMat = m_preprocImg->adjustBrightness(*m_imgMat, value);
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
        *m_imgMat = m_preprocImg->adjustContrast(*m_imgMat_orig, value);
    else
        *m_imgMat = m_preprocImg->adjustContrast(*m_imgMat, value);
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
        m_preprocImg->applyThreshold(*m_imgMat, 128, true, 1);
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
        *m_imgMat = m_preprocImg->manualThreshold(*m_imgMat_orig, value, false, 1);
    else
        *m_imgMat = m_preprocImg->manualThreshold(*m_imgMat, value, false, 1);
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
                              m_pixmapItem->pixmap().rect());
        } else {
            //delete m_rect;
            removeItems(*m_scene, "rect");
            rects_size = 0;
            m_rect
                = new MyRectF(m_scene,
                              ui->graphicsView->mapToScene(ui->graphicsView->getProps().m_topleft),
                              ui->graphicsView->mapToScene(ui->graphicsView->getProps().m_topleft),
                              ++rects_size,
                              m_pixmapItem->pixmap().rect());
        }
    }

    //m_ocrrect->setTopLeft(ui->graphicsView->mapToScene(ui->graphicsView->getProps().m_topleft));
}

void MainWindow::endRect()
{
    if (ui->graphicsView->dragMode() == QGraphicsView::RubberBandDrag && !previewpreproc) {
        m_rect->setBottomRight(
            ui->graphicsView->mapToScene(ui->graphicsView->getProps().m_bottomright));
        m_scene->addItem(m_rect);
        m_scene->items().first()->setData(0, "rect");
        m_scene->items().first()->setData(1, m_rect->getRect());
        ui->label->setText("You are selecting the area for recognize. Press SHIFT for multiselect "
                           "or If you want clear areas press DELETE.");
        if (m_rect->getRect().height() < 20)
            rects_size = 0;
    }
}

void MainWindow::on_actionOpenImage_triggered()
{
    filename = QFileDialog::getOpenFileName(this,
                                            "Please Select The Image",
                                            "",
                                            tr("Image Files (*.png *.jpg *.bmp)"));
    openImage();
}

void MainWindow::on_actionPreprocess_toggled(bool arg1)
{
    QIcon icon1;

    if (arg1) {
        showPreprocess();
        if (isDark) {
            icon1.addFile(QString::fromUtf8(
                              ":/breeze-icons-master/icons-dark/actions/32/trim-margins.svg"),
                          QSize(),
                          QIcon::Normal,
                          QIcon::Off);
            stateicon2
                .addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons-dark/actions/32/trim-to-selection.svg"),
                         QSize(),
                         QIcon::Disabled,
                         QIcon::Off);
        } else {
            icon1.addFile(QString::fromUtf8(
                              ":/breeze-icons-master/icons/actions/32/trim-margins.svg"),
                          QSize(),
                          QIcon::Normal,
                          QIcon::Off);
            stateicon2.addFile(QString::fromUtf8(
                                   ":/breeze-icons-master/icons/actions/32/trim-to-selection.svg"),
                               QSize(),
                               QIcon::Disabled,
                               QIcon::Off);
        }

    } else {
        applyPreprocess();
        if (isDark) {
            icon1.addFile(QString::fromUtf8(
                              ":/breeze-icons-master/icons-dark/actions/32/trim-to-selection.svg"),
                          QSize(),
                          QIcon::Normal,
                          QIcon::Off);
        } else {
            icon1.addFile(QString::fromUtf8(
                              ":/breeze-icons-master/icons/actions/32/trim-to-selection.svg"),
                          QSize(),
                          QIcon::Normal,
                          QIcon::Off);
        }
    }

    ui->actionPreprocess->setIcon(icon1);
    stateicon1 = icon1;
}

void MainWindow::on_actionRotate_toggled(bool arg1)
{
    m_rwidget->move(this->geometry().x() + 20, this->geometry().y() + 40);

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

    m_ocrWidget->move(this->geometry().x() + 90, this->geometry().y() + 40);
    if (rects_size < 1)
        m_images->prepend((getImage(*m_scene, m_pixmapItem->pixmap().rect())));
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
    *m_imgQ = getImage(*m_scene, m_pixmapItem->pixmap().rect());
    *m_imgMat_orig = ConvertImage::qimageToMatRef(*m_imgQ, CV_8UC4);
    *m_imgMat = *m_imgMatTh = *m_imgMat_orig;
    m_adjWidget->move(this->geometry().x() + 70, this->geometry().y() + 40);
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
    m_settwidget->move(this->geometry().right() - 380, this->geometry().y() + 40);
    if (arg1)
        m_settwidget->show();
    else
        m_settwidget->hide();
}

void MainWindow::on_actionClear_recognize_areas_triggered()
{
    removeItems(*m_scene, "rect");
    rects_size = 0;
}
