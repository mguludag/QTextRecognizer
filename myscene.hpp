#ifndef MYSCENE_HPP
#define MYSCENE_HPP

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QWidget>

class MyScene : public QGraphicsScene
{
    Q_OBJECT
    struct Props
    {
        QPointF m_pos, m_topleft, m_bottomright;
    } Props;

public:
    explicit MyScene(QWidget *parent = nullptr);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    struct Props getProps() { return Props; }

signals:
    void moveSignal();
    void pressSignal();
    void releaseSignal();

public slots:
private:
};

#endif // MYSCENE_HPP
