#ifndef MYGRAPHICSVIEW_HPP
#define MYGRAPHICSVIEW_HPP

#include "mypixmapitem.hpp"
#include "myrectf.hpp"
#include <QGraphicsView>

#include <QMouseEvent>
#include <QObject>
#include <QWidget>

class MyGraphicsViewTool;

class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT
    struct Props
    {
        QPoint m_pos, m_topleft, m_bottomright;
    } Props;

public:
    explicit MyGraphicsView(QWidget *parent = nullptr);
    void setTool(MyGraphicsViewTool *tool) { m_tool = tool; }
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    struct Props getProps() { return Props; }
    void rotate(MyPixmapItem &m_imageItem, QGraphicsScene &m_scene, double angle);
    void resetAngle();

signals:
    void moveSignal();
    void pressSignal();
    void releaseSignal();
    void drop();

public slots:

protected:
private:
    MyGraphicsViewTool *m_tool = nullptr;
    double m_angle = 0;
};

class MyGraphicsViewTool : public QObject
{
public:
    MyGraphicsViewTool(MyGraphicsView *displayer, QObject *parent = nullptr)
        : QObject(parent)
        , m_displayer(displayer)
    {}
    virtual ~MyGraphicsViewTool() {}
    virtual void mousePressEvent(QMouseEvent * /*event*/) {}
    virtual void mouseMoveEvent(QMouseEvent * /*event*/) {}
    virtual void mouseReleaseEvent(QMouseEvent * /*event*/) {}
    virtual void resolutionChanged(double /*factor*/) {}
    virtual void rotationChanged(double /*delta*/) {}
    virtual void reset() {}

    MyGraphicsView *getDisplayer() const { return m_displayer; }

protected:
    MyGraphicsView *m_displayer;
};

#endif // MYGRAPHICSVIEW_HPP
