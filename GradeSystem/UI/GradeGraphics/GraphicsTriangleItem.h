#pragma once
#include "preheader.h"
#include "GraphicsRectItem.h"
class GraphicsTriangleItem : public GraphicsRectItem
{
public:
    GraphicsTriangleItem(const QRect & rect ,QGraphicsItem * parent);
    QPainterPath shape() const;


protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};
