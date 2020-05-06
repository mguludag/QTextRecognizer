#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "aboutdialog.hpp"
#include "adjustwidget.hpp"
#include "graphics_view_zoom.hpp"
#include "mygraphicsview.hpp"
#include "mypoint.h"
#include "mypolyf.hpp"
#include "myrectf.hpp"
#include "myscene.hpp"
#include "ocrwidget.hpp"
#include "rotatewidget.hpp"
#include "settingswidget.hpp"

#include "ui_mainwindow.h"

#include <QClipboard>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QList>
#include <QMainWindow>

#include <QMenu>
#include <QPixmap>
#include <QTextStream>
#include <QToolBar>
#include <QToolTip>

#define PI 22 / 7

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initMainTbar();
    void initDockTBar();
    void initPoints();
    void initDrawTools();
    void init();
    void enableDisableActions(QAction *except, bool enabled);
    void drawPolygon();
    void checkPoints();
    void removeItems(QGraphicsScene &scene, QString name);
    void addItems();
    QImage getImage(QGraphicsScene &scene, const QRectF &_rect);
    QPointF getRotatedPoint(QPointF p, QPointF center, qreal angleRads);
    QRectF getMinimumBoundingRect(QRectF r, qreal angleRads);

public slots:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void showEvent(QShowEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *event);
    void moveEvent(QMoveEvent *);

    void getPoints();
    void showPreprocess();
    void applyPreprocess();
    void autoDetectLayout();
    void hidePopups();
    void rotatePixmap();
    void centerImage();
    void cancelPreprocess();
    void setBrightness();
    void setContrast();
    void setAThreshold();
    void setMThreshold();
    void acceptAdjust();
    void rejectAdjust();
    void showResult();
    void onProgress();
    void writePos();
    void beginRect();
    void endRect();
    void openImage();
    void getFilename();
    qreal degToRad(qreal deg);
    QRectF getRotatedRect(QRectF _rect);

    void on_actionOpenImage_triggered();
    void on_actionPreprocess_triggered();
    void on_actionRotate_toggled(bool arg1);
    void on_actionBest_Fit_triggered();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionRecognize_text_toggled(bool arg1);
    void on_actionAdjust_image_toggled(bool checked);
    void on_actionCopy_to_clipboard_triggered();
    void on_actionClear_text_triggered();
    void on_actionSave_triggered();
    void on_actionAbout_triggered();
    void on_actionSettings_toggled(bool arg1);
    void on_actionAuto_detect_layout_triggered();
    void on_actionClear_layout_triggered();

private:
    Ui::MainWindow *ui;
    enum icons { light, dark };
    bool isDark = false, isFile = false;
    bool cancelpreprocess = false, onprogress = false, previewpreproc = false, autolayout = false;
    bool state = false;
    bool rem = false;
    bool pressShift = false;
    bool b_cont = true, c_cont = true, at_cont = true, mt_cont = true;
    int key = 0, rects_size = 0;
    double m_angle = 0;
    QMenu *m_menu = nullptr, *m_menu_clearlayout;
    QToolBar *m_DockTBar = nullptr;
    QString filename;
    QClipboard *m_clipboard = QGuiApplication::clipboard();
    QPixmap *m_pixmap = nullptr, *t_pixmap = nullptr;
    MyScene *m_scene = nullptr;
    MyPixmapItem *m_pixmapItem = nullptr;
    QImage *m_imgQ = nullptr, *m_imgQ_copy = nullptr;
    cv::Mat *m_imgMat = nullptr, *m_imgMat_orig = nullptr, *m_imgMatTh = nullptr;
    QElapsedTimer *m_etimer = nullptr;
    MyPoint *m_pts[4] = {nullptr};
    MyPolyF *m_polyf = nullptr;
    MyRectF *m_rect = nullptr;
    QPen m_pen;
    QPen m_pen1;
    QPen m_pen2;
    QBrush m_brush;
    QVector<QPointF> m_pointQ = {};
    QList<QImage> *m_images = nullptr;
    std::vector<cv::Point2f> m_pointStd = {};
    Settings *m_settings = nullptr;
    RotateWidget *m_rwidget = nullptr;
    adjustWidget *m_adjWidget = nullptr;
    OCRWidget *m_ocrWidget = nullptr;
    Graphics_view_zoom *z = nullptr;
    SettingsWidget *m_settwidget = nullptr;
    tesseract::TessBaseAPI *tess = nullptr;

public slots:
    void setIcons()
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
                             ":/breeze-icons-master/icons-dark/actions/32/trim-margins.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionPreprocess->setIcon(icon);
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons-dark/actions/32/edit-undo.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionRotate_Left->setIcon(icon);
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons-dark/actions/32/edit-redo.svg"),
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
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons-dark/actions/32/zoom-in.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionZoom_In->setIcon(icon);
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons-dark/actions/32/zoom-out.svg"),
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
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons-dark/actions/32/configure.svg"),
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
            icon.addFile(
                QString::fromUtf8(
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
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons-dark/actions/32/dialog-ok-apply.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionAccept->setIcon(icon);
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons-dark/actions/32/dialog-cancel.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionCancel->setIcon(icon);
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons-dark/actions/32/trim-to-selection.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionAuto_detect_layout->setIcon(icon);
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons-dark/actions/32/edit-delete.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionClear_layout->setIcon(icon);
        } else {
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons/actions/32/document-open.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionOpenImage->setIcon(icon);
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons/actions/32/trim-margins.svg"),
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
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons/actions/32/zoom-fit-best.svg"),
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
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons/actions/32/document-new.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionCopy_to_clipboard->setIcon(icon);
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons/actions/32/document-close.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionClear_text->setIcon(icon);
            icon.addFile(
                QString::fromUtf8(
                    ":/breeze-icons-master/icons/actions/32/document-edit-decrypt-verify.svg"),
                QSize(),
                QIcon::Normal,
                QIcon::Off);
            ui->actionRecognize_text->setIcon(icon);
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons/actions/32/view-refresh.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionRotate->setIcon(icon);
            icon.addFile(QString::fromUtf8(":/breeze-icons-master/icons/actions/32/configure.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionSettings->setIcon(icon);
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons/actions/32/help-whatsthis.svg"),
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
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons/actions/32/document-save.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionSave->setIcon(icon);
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons/actions/32/dialog-ok-apply.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionAccept->setIcon(icon);
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons/actions/32/dialog-cancel.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionCancel->setIcon(icon);
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons/actions/32/trim-to-selection.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionAuto_detect_layout->setIcon(icon);
            icon.addFile(QString::fromUtf8(
                             ":/breeze-icons-master/icons/actions/32/edit-delete.svg"),
                         QSize(),
                         QIcon::Normal,
                         QIcon::Off);
            ui->actionClear_layout->setIcon(icon);
        }
    }
private slots:
};

#endif // MAINWINDOW_HPP
