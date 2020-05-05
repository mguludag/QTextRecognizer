#ifndef MYSCENE_HPP
#define MYSCENE_HPP

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMimeData>
#include <QObject>
#include <QUrl>
#include <QWidget>

class MyScene : public QGraphicsScene
{
    Q_OBJECT
    struct Props
    {
        QPointF m_pos, m_topleft, m_bottomright;
    } Props;

public:
    QString filename;
    explicit MyScene(QWidget *parent = nullptr);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    struct Props getProps() { return Props; }

signals:
    void moveSignal();
    void pressSignal();
    void releaseSignal();
    void drop();

public slots:

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event)
    {
        if (event->mimeData()->hasUrls()) {
            event->acceptProposedAction();
        }
    }
    void dragMoveEvent(QGraphicsSceneDragDropEvent *) {}
    void dropEvent(QGraphicsSceneDragDropEvent *event)
    {
        filename = event->mimeData()->urls()[0].toLocalFile();
        event->acceptProposedAction();
        emit drop();
    }

private:
};

#endif // MYSCENE_HPP
