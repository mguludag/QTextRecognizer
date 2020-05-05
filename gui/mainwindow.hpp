#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "aboutdialog.hpp"
#include "adjustwidget.hpp"
#include "graphics_view_zoom.hpp"
#include "mygraphicsview.hpp"
#include "myicon.hpp"
#include "mypoint.h"
#include "mypolyf.hpp"
#include "myrectf.hpp"
#include "myscene.hpp"
#include "ocrwidget.hpp"
#include "rotatewidget.hpp"
#include "settingswidget.hpp"

#include <QClipboard>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QList>
#include <QMainWindow>

#include <QPixmap>
#include <QSpacerItem>
#include <QTextStream>
#include <QTimer>
#include <QToolBar>
#include <QToolTip>
#include <QtDebug>

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
    void showPreprocess();
    void applyPreprocess();
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
    void hidePopups();
    void rotatePixmap();
    void centerImage();
    void cancelPreprocess();
    void blinkActionPreprocess();
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
    void setIcons();
    void openImage();
    void getFilename();
    qreal degToRad(qreal deg);
    QRectF getRotatedRect(QRectF _rect);

    void on_actionOpenImage_triggered();
    void on_actionPreprocess_toggled(bool arg1);
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
    void on_actionClear_recognize_areas_triggered();
private slots:

private:
    Ui::MainWindow *ui;
    enum icons { light, dark };
    bool isDark = false, isFile = false;
    bool cancelpreprocess = false, onprogress = false, previewpreproc = false;
    bool state = false;
    bool rem = false;
    bool pressShift = false;
    bool b_cont = true, c_cont = true, at_cont = true, mt_cont = true;
    int key = 0, rects_size = 0;
    double m_angle = 0;
    QIcon stateicon1, stateicon2;
    QToolBar *m_DockTBar = nullptr;
    QString filename;
    QClipboard *m_clipboard = QGuiApplication::clipboard();
    QPixmap *m_pixmap = nullptr, *t_pixmap = nullptr;
    MyScene *m_scene = nullptr;
    MyPixmapItem *m_pixmapItem = nullptr;
    QPoint m_mousePos;
    QImage *m_imgQ = nullptr, *m_imgQ_copy = nullptr;
    cv::Mat *m_imgMat = nullptr, *m_imgMat_orig = nullptr, *m_imgMatTh = nullptr;
    QTimer *m_timer = nullptr;
    QElapsedTimer *m_etimer = nullptr;
    MyPoint *m_pts[4];
    MyPolyF *m_polyf = nullptr;
    MyRectF *m_rect = nullptr;
    QPen m_pen;
    QPen m_pen1;
    QPen m_pen2;
    QBrush m_brush;
    QVector<QPointF> m_pointQ;
    QList<QImage> *m_images = nullptr;
    std::vector<cv::Point2f> m_pointStd;
    Settings *m_settings = nullptr;
    RotateWidget *m_rwidget = nullptr;
    adjustWidget *m_adjWidget = nullptr;
    PreprocessImage *m_preprocImg = nullptr;
    OCRWidget *m_ocrWidget = nullptr;
    Graphics_view_zoom *z;
    SettingsWidget *m_settwidget;
    tesseract::TessBaseAPI *tess = nullptr;
};

#endif // MAINWINDOW_HPP
