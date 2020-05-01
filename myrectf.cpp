#include "myrectf.hpp"

MyRectF::MyRectF(QGraphicsScene *sceneWidget,
                 QPointF _topleft,
                 QPointF _bottomright,
                 int _number,
                 QRectF _brect,
                 QObject *parent)
    : QObject(parent)
    , QGraphicsItem()
    , scene(sceneWidget)
{
    rect.setTopLeft(_topleft);
    rect.setBottomRight(_bottomright);
    number = _number;
    boundingrect = _brect;
}

QRectF MyRectF::boundingRect() const
{
    return boundingrect;
}

QPainterPath MyRectF::shape() const
{
    QPainterPath path;
    path.addRect(rect);
    return path;
}

void MyRectF::setBottomRight(QPointF _bottomright)
{
    rect.setBottomRight(_bottomright);
}

QRectF MyRectF::getRect()
{
    return rect;
}

void MyRectF::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QPalette().highlight());
    painter->setOpacity(0.4);
    painter->setPen(QPen(QPalette().highlight(), 2));
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->drawRect(rect);
    QRectF numrect;
    QPointF numtopleft;
    numtopleft = QPointF((std::min(rect.topLeft().x(), rect.bottomRight().x())),
                         std::min(rect.topLeft().y(), rect.bottomRight().y()));
    numrect.setTopLeft(numtopleft);
    if (rect.height() > 20 || rect.height() < -20)
        numrect.setBottomRight(QPointF(numtopleft.x() + 30, numtopleft.y() + 30));
    else
        numrect.setBottomRight(QPointF(numtopleft.x(), numtopleft.y()));
    painter->setBrush(QPalette().highlight());
    painter->setPen(Qt::NoPen);
    painter->setOpacity(0.9);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->drawRect(numrect);

    if (rect.height() > 20 || rect.height() < -20) {
        QFont font;
        font.setPixelSize(static_cast<int>(numrect.height()));
        font.setBold(true);
        painter->setFont(font);
        painter->setPen(QPalette().highlightedText().color());
        painter->drawText(numrect, Qt::AlignCenter, QString::number(number));
    }
}
