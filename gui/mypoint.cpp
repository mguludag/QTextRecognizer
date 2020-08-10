/*****************************************************************************
 * mypoint.cpp
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
#include "mypoint.h"

MyPoint::MyPoint(QGraphicsScene *sceneWidget, QObject *parent)
    : QObject(parent)
    , QGraphicsItem()
    , scene(sceneWidget)
{
    setFlag(ItemIsMovable);
}

MyPoint::~MyPoint() {}

void MyPoint::setRatio(int val)
{
    ratio = val;
}

QPointF MyPoint::getPos()
{
    return m_pos;
}

QRectF MyPoint::boundingRect() const
{
    return QRectF(-ratio / 2, -ratio / 2, ratio, ratio);
}

void MyPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->setOpacity(0.5);
    painter->drawEllipse(-ratio / 2, -ratio / 2, ratio, ratio);

    QRadialGradient gradient(-ratio / 6, -ratio / 6, ratio / 2);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(ratio / 6, ratio / 6);
        gradient.setFocalPoint(ratio / 6, ratio / 6);
        gradient.setColorAt(1, QColor(Qt::yellow).lighter(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).lighter(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-ratio / 2, -ratio / 2, ratio, ratio);
}

/* Override method of capturing a mouse click event, add the sending signal from the object
 * */
void MyPoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();

    QGraphicsItem::mousePressEvent(event);
}

void MyPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();

    // Call the parent function mouse click events
    QGraphicsItem::mouseReleaseEvent(event);
    newPos = pos();
}

void MyPoint::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    /* Set the position of the graphical element in the graphic scene, 
     * translate coordinates of the cursor within the graphic element 
     * in the coordinate system of the graphic scenes
     * */
    emit signal1();
    this->setPos(mapToScene(event->pos()));
    QGraphicsItem::mouseMoveEvent(event);
}
