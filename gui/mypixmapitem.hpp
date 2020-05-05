#ifndef MYPIXMAPITEM_HPP
#define MYPIXMAPITEM_HPP

#include <QGraphicsPixmapItem>
#include <QObject>

class MyPixmapItem : public QObject, public QGraphicsPixmapItem
{
public:
    MyPixmapItem(QObject *parent = nullptr);
    void prepareGeoChanges();
    virtual void rotationChanged(double delta);
};

#endif // MYPIXMAPITEM_HPP
