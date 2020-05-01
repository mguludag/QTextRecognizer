#include "mypoint.h"
#include <QDebug>

MyPoint::MyPoint(QGraphicsScene *sceneWidget, QObject *parent)
    : QObject(parent)
    , QGraphicsItem()
    , scene(sceneWidget)
{
    setFlag(ItemIsMovable);
    //setFlag(ItemSendsGeometryChanges);
    //setCacheMode(DeviceCoordinateCache);
    //setZValue(-1);
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
