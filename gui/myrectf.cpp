#include "myrectf.hpp"

MyRectF::MyRectF() {}

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

void MyRectF::setTopLeft(QPointF _topleft)
{
    rect.setTopLeft(_topleft);
}

void MyRectF::setBottomRight(QPointF _bottomright)
{
    rect.setBottomRight(_bottomright);
}

void MyRectF::setBoundingRect(QRectF _brect)
{
    boundingrect = _brect;
}

QRectF MyRectF::getRect()
{
    return rect;
}

void MyRectF::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QPalette().highlight());
    painter->setOpacity(0.35);
    painter->setPen(QPen(QPalette().highlight(), 2));
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->drawRect(rect);
    QRectF numrect;
    QPointF numtopleft;

    numtopleft = QPointF((std::min(rect.topLeft().x(), rect.bottomRight().x())),
                         std::min(rect.topLeft().y(), rect.bottomRight().y()));
    numrect.setTopLeft(numtopleft);
    if (rect.height() > 10 || rect.height() < -10)
        numrect.setBottomRight(QPointF(numtopleft.x() + abs(rect.height()) * 0.30,
                                       numtopleft.y() + abs(rect.height()) * 0.25));
    else
        numrect.setBottomRight(QPointF(numtopleft.x(), numtopleft.y()));
    painter->setBrush(QPalette().highlight());
    painter->setPen(Qt::NoPen);
    painter->setOpacity(0.75);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
    painter->drawRect(numrect);

    if (rect.height() > 10 || rect.height() < -10) {
        QFont font;
        font.setPixelSize(static_cast<int>(numrect.height()));
        font.setBold(true);
        painter->setFont(font);
        painter->setPen(QPalette().highlightedText().color());
        painter->drawText(numrect, Qt::AlignCenter, QString::number(number));
    }
}
