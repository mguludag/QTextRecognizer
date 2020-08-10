/*****************************************************************************
 * mygraphicsview.cpp
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
#include "mygraphicsview.hpp"

MyGraphicsView::MyGraphicsView(QWidget *parent)
{
    setRenderHint(QPainter::Antialiasing);
    setTool(m_tool);
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    Props.m_pos = event->pos();
    QGraphicsView::mouseMoveEvent(event);
    emit moveSignal();
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    Props.m_topleft = event->pos();
    QGraphicsView::mousePressEvent(event);
    emit pressSignal();
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    Props.m_bottomright = event->pos();
    QGraphicsView::mouseReleaseEvent(event);
    emit releaseSignal();
}

void MyGraphicsView::rotate(MyPixmapItem &m_imageItem, QGraphicsScene &m_scene, double angle)
{
    angle = angle < 0.0 ? angle + 360.0 : angle >= 360.0 ? angle - 360.0 : angle;
    m_imageItem.prepareGeoChanges();
    m_imageItem.setRotation(angle);
    m_scene.setSceneRect(m_imageItem.sceneBoundingRect());
    fitInView(m_scene.sceneRect(), Qt::KeepAspectRatio);
}

void MyGraphicsView::resetAngle()
{
    m_angle = 0;
}
