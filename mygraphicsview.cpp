#include "mygraphicsview.hpp"

MyGraphicsView::MyGraphicsView(QWidget *parent)
{
    setMouseTracking(true);
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

void MyGraphicsView::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MyGraphicsView::dropEvent(QDropEvent *event)
{
    filename = event->mimeData()->urls()[0].toLocalFile();
    event->acceptProposedAction();
    emit drop();
}
