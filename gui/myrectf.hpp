#ifndef MYRECTF_HPP
#define MYRECTF_HPP

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QObject>
#include <QPainter>
#include <QRectF>
#include <QStyleOption>

class MyRectF : public QObject, public QGraphicsItem
{
public:
    explicit MyRectF();
    explicit MyRectF(QGraphicsScene *sceneWidget,
                     QPointF _topleft,
                     QPointF _bottomright,
                     int _number,
                     QRectF _brect,
                     QObject *parent = nullptr);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setTopLeft(QPointF _topleft);
    void setBottomRight(QPointF _bottomright);
    void setBoundingRect(QRectF _brect);
    QRectF getRect();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QGraphicsScene *scene;
    int number;
    QPointF topleft, bottomright;
    QRectF boundingrect;
    QRectF rect;
};

#endif // MYRECTF_HPP
