#ifndef MYPOINT_H
#define MYPOINT_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QPainter>
#include <QStyleOption>

/* To work signals and slots, we inherit from QObject,
 * */
class MyPoint : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit MyPoint(QGraphicsScene *sceneWidget, QObject *parent = nullptr);
    ~MyPoint();
    void setRatio(int val = 20);
    QPointF getPos();

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:
    /* Signal to be sent in the event that there was a mouse click on the object
     * */
    void signal1();
    void signal2();

protected:
private:
    /* These methods are virtual, so they need to be implemented 
     * in the case of inheritance from QGraphicsItem
     * */
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPointF newPos, m_pos;
    QGraphicsScene *scene;
    int ratio;
};

#endif // MYPOINT_H
