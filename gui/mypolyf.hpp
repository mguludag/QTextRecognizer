/*****************************************************************************
 * mypolyf.hpp
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
#ifndef MYPOLYF_HPP
#define MYPOLYF_HPP

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QPainter>
#include <QStyleOption>

class MyPolyF : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit MyPolyF(QGraphicsScene *sceneWidget, QObject *parent = nullptr);
    ~MyPolyF();
    void setPoly(QVector<QPointF> &pt);

    void setBrect(const QRectF &value);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    QPolygonF getPolyF();

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:
    /* Signal to be sent in the event that there was a mouse click on the object
     * */
    void signal2();

protected:
private:
    /* These methods are virtual, so they need to be implemented 
     * in the case of inheritance from QGraphicsItem
     * */

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPointF newPos;
    QGraphicsScene *scene;
    QPolygonF *poly;
    QRectF brect;
};

#endif // MYPOLYF_HPP
