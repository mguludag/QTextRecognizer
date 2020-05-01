#ifndef MYGRAPHICSVIEW_HPP
#define MYGRAPHICSVIEW_HPP

#include <QGraphicsView>
#include <QMimeData>
#include <QMouseEvent>
#include <QObject>
#include <QWidget>

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
    struct Props
    {
        QPoint m_pos, m_topleft, m_bottomright;
    } Props;

public:
    QString filename;
    explicit MyGraphicsView(QWidget *parent = nullptr);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *event);
    struct Props getProps() { return Props; }

signals:
    void moveSignal();
    void pressSignal();
    void releaseSignal();
    void drop();

public slots:

private:
};

#endif // MYGRAPHICSVIEW_HPP
