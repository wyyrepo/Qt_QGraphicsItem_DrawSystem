#include "GraphicsTriangleItem.h"

GraphicsTriangleItem::GraphicsTriangleItem(const QRect &rect, QGraphicsItem *parent)
    :GraphicsRectItem(rect,parent)
{
}

QPainterPath GraphicsTriangleItem::shape() const
{
    QPainterPath path;
    QRectF rc = rect();
    QPointF middle =QPointF((rc.left() + rc.right())*0.5,0);

    QPolygonF item;
    item <<rc.bottomLeft()<<middle<< rc.bottomRight()<<rc.bottomLeft();
    path.addPolygon(item);
    return qt_graphicsItem_shapeFromPath(path,_pen);
}

void GraphicsTriangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rc = rect();

    QPointF middle =QPointF((rc.left() + rc.right())*0.5,0);

    QPolygonF item;
    item <<rc.bottomLeft()<<middle<< rc.bottomRight()<<rc.bottomLeft();
    QPen pen =painter->pen();
    pen.setWidth(4);
    pen.setColor(m_color);
    painter->setPen(pen);
    painter->drawPolygon(item);

}

