/*****************************************************************************
 * mygraphicsview.hpp
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
