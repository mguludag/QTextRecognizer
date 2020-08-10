/*****************************************************************************
 * myrectf.hpp
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
#ifndef MYRECTF_HPP
#define MYRECTF_HPP

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>
#include <QPainter>
#include <QRectF>
#include <QStyleOption>

class MyRectF : public QObject, public QGraphicsItem
{
public:
    explicit MyRectF();
    explicit MyRectF(QGraphicsScene *sceneWidget,
                     QPointF _topleft,
                     QPointF _bottomright,
                     int _number,
                     QRectF _brect,
                     QObject *parent = nullptr);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setTopLeft(QPointF _topleft);
    void setBottomRight(QPointF _bottomright);
    void setBoundingRect(QRectF _brect);
    QRectF getRect();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QGraphicsScene *scene;
    int number;
    QPointF topleft, bottomright;
    QRectF boundingrect;
    QRectF rect;
};

#endif // MYRECTF_HPP
