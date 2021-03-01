#include "myscene.hpp"

MyScene::MyScene(QWidget *parent) {}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Props.m_pos = event->pos();
    emit moveSignal();
    QGraphicsScene::mouseMoveEvent(event);
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Props.m_topleft = event->pos();
    QGraphicsScene::mousePressEvent(event);
    emit pressSignal();
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Props.m_bottomright = event->pos();
    QGraphicsScene::mouseReleaseEvent(event);
    emit releaseSignal();
}
