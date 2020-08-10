/*****************************************************************************
 * myscene.hpp
 *
 * Created: 5/28/2020 2020 by mguludag
 *
 * Copyright 2020 mguludag. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *****************************************************************************/
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
