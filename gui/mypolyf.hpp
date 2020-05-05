#ifndef MYPOLYF_HPP
#define MYPOLYF_HPP

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QPainter>
#include <QStyleOption>

class MyPolyF : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit MyPolyF(QGraphicsScene *sceneWidget, QObject *parent = nullptr);
    ~MyPolyF();
    void setPoly(QVector<QPointF> &pt);

    void setBrect(const QRectF &value);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    QPolygonF getPolyF();

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:
    /* Signal to be sent in the event that there was a mouse click on the object
     * */
    void signal2();

protected:
private:
    /* These methods are virtual, so they need to be implemented 
     * in the case of inheritance from QGraphicsItem
     * */

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPointF newPos;
    QGraphicsScene *scene;
    QPolygonF *poly;
    QRectF brect;
};

#endif // MYPOLYF_HPP
