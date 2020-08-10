/*****************************************************************************
 * myrectf.cpp
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
#include "myrectf.hpp"

MyRectF::MyRectF() {}

MyRectF::MyRectF(QGraphicsScene *sceneWidget,
                 QPointF _topleft,
                 QPointF _bottomright,
                 int _number,
                 QRectF _brect,
                 QObject *parent)
    : QObject(parent)
    , QGraphicsItem()
    , scene(sceneWidget)
{
    rect.setTopLeft(_topleft);
    rect.setBottomRight(_bottomright);
    number = _number;
    boundingrect = _brect;
}

QRectF MyRectF::boundingRect() const
{
    return boundingrect;
}

QPainterPath MyRectF::shape() const
{
    QPainterPath path;
    path.addRect(rect);
    return path;
}

void MyRectF::setTopLeft(QPointF _topleft)
{
    rect.setTopLeft(_topleft);
}

void MyRectF::setBottomRight(QPointF _bottomright)
{
    rect.setBottomRight(_bottomright);
}

void MyRectF::setBoundingRect(QRectF _brect)
{
    boundingrect = _brect;
}

QRectF MyRectF::getRect()
{
    return rect;
}

void MyRectF::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QPalette().highlight());
    painter->setOpacity(0.35);
    painter->setPen(QPen(QPalette().highlight(), 2));
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->drawRect(rect);
    QRectF numrect;
    QPointF numtopleft;

    numtopleft = QPointF((std::min(rect.topLeft().x(), rect.bottomRight().x())),
                         std::min(rect.topLeft().y(), rect.bottomRight().y()));
    numrect.setTopLeft(numtopleft);
    if (rect.height() > 10 || rect.height() < -10)
        numrect.setBottomRight(QPointF(numtopleft.x() + abs(rect.height()) * 0.30,
                                       numtopleft.y() + abs(rect.height()) * 0.25));
    else
        numrect.setBottomRight(QPointF(numtopleft.x(), numtopleft.y()));
    painter->setBrush(QPalette().highlight());
    painter->setPen(Qt::NoPen);
    painter->setOpacity(0.75);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->drawRect(numrect);

    if (rect.height() > 10 || rect.height() < -10) {
        QFont font;
        font.setPixelSize(static_cast<int>(numrect.height()));
        font.setBold(true);
        painter->setFont(font);
        painter->setPen(QPalette().highlightedText().color());
        painter->drawText(numrect, Qt::AlignCenter, QString::number(number));
    }
}
