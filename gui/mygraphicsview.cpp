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
    //fitInView(m_scene.sceneRect(), Qt::KeepAspectRatio);
}

void MyGraphicsView::resetAngle()
{
    m_angle = 0;
}
