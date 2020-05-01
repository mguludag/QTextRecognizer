#include "mypolyf.hpp"

MyPolyF::MyPolyF(QGraphicsScene *sceneWidget, QObject *parent)
    : QObject(parent)
    , QGraphicsItem()
    , scene(sceneWidget)
{
    //setFlag(ItemIsMovable);
}

MyPolyF::~MyPolyF() {}

void MyPolyF::setPoly(QVector<QPointF> &pt)
{
    poly = new QPolygonF(pt);
}

void MyPolyF::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();

    QGraphicsItem::mousePressEvent(event);
}

void MyPolyF::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();

    // Call the parent function mouse click events
    QGraphicsItem::mouseReleaseEvent(event);
}

void MyPolyF::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    this->setPos(mapToScene(event->pos()));
    emit signal2();
    QGraphicsItem::mouseMoveEvent(event);
}

QRectF MyPolyF::boundingRect() const
{
    return brect;
}

QPainterPath MyPolyF::shape() const
{
    QPainterPath path;
    path.addPolygon(*poly);
    return path;
}

QPolygonF MyPolyF::getPolyF()
{
    return *poly;
}

void MyPolyF::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPolygonF subpoly(QPolygonF(sceneBoundingRect()).subtracted(*poly));
    painter->setBrush(Qt::black);
    painter->setOpacity(0.7);
    painter->drawPolygon(subpoly, Qt::OddEvenFill);
    painter->setBrush(Qt::BrushStyle::NoBrush);

    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, QColor(Qt::yellow).lighter(120));
        gradient.setColorAt(0, QColor(Qt::darkYellow).lighter(120));
    } else {
        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);
    }
    painter->setPen(QPen(gradient, 6));
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->drawPolygon(*poly);
}

void MyPolyF::setBrect(const QRectF &value)
{
    brect = value;
}
