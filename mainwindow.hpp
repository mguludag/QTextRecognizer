#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "aboutdialog.hpp"
#include "adjustwidget.hpp"
#include "graphics_view_zoom.h"
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

    void drawPolygon();
    void enableDisableActions(QAction *except, bool enabled);

    void showPreprocess();
    void applyPreprocess();
    void removeItems(QGraphicsScene &scene, QString name);
    void addItems();
    QImage getImage(QGraphicsScene &scene, const QRectF &rect);

public slots:
    void getPoints();
    void on_actionOpenImage_triggered();
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void showEvent(QShowEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent *event);
    void moveEvent(QMoveEvent *);
    void on_actionPreprocess_toggled(bool arg1);
    void rotatePixmap();
    void centerImage();
    void cancelPreprocess();
    void on_actionRotate_toggled(bool arg1);
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

private slots:
    void on_actionClear_recognize_areas_triggered();

private:
    Ui::MainWindow *ui;
    enum icons { light, dark };
    bool isDark = false, isFile = false;
    bool cancelpreprocess = false, onprogress = false, previewpreproc = false;
    bool state = false;
    bool pressShift = false;
    bool b_cont = true, c_cont = true, at_cont = true, mt_cont = true;
    int key = 0, rects_size = 0;
    QIcon stateicon1, stateicon2;
    //QSpacerItem *m_hSpcrMainTBar;
    QToolBar *m_DockTBar;
    QString filename;
    QClipboard *m_clipboard = QGuiApplication::clipboard();
    QPixmap *m_pixmap;
    MyScene *m_scene;
    QGraphicsPixmapItem *m_pixmapItem;
    QPoint m_mousePos;
    QImage *m_imgQ, *m_imgQ_copy;
    cv::Mat *m_imgMat, *m_imgMat_orig, *m_imgMatTh;
    QTimer *m_timer;
    QElapsedTimer *m_etimer;
    MyPoint *m_pts[4];
    MyPolyF *m_polyf;
    MyRectF *m_rect;
    QPen m_pen;
    QPen m_pen1;
    QPen m_pen2;
    QRectF *m_ocrrect;
    QBrush m_brush;
    QVector<QPointF> m_pointQ;
    QList<QImage> *m_images;
    std::vector<cv::Point2f> m_pointStd;
    Settings *m_settings;
    RotateWidget *m_rwidget;
    adjustWidget *m_adjWidget;
    PreprocessImage *m_preprocImg;
    OCRWidget *m_ocrWidget;
    Graphics_view_zoom *z;
    SettingsWidget *m_settwidget;
    tesseract::TessBaseAPI *tess;
};

#endif // MAINWINDOW_HPP
