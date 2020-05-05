#include "mypixmapitem.hpp"

MyPixmapItem::MyPixmapItem(QObject *parent)
{
}

void MyPixmapItem::prepareGeoChanges()
{
    prepareGeometryChange();
}

void MyPixmapItem::rotationChanged(double delta)
{
    QTransform t;
    t.rotate(delta);
}
