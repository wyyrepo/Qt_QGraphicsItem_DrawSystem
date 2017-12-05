#pragma once
#include "preheader.h"
#include "GraphicsRectItem.h"

class GraphicsItemGroup : public GraphicsRectItem
{
public:
    GraphicsItemGroup ( QGraphicsItemGroup * group, QGraphicsItem * parent );
    QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF & point );
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QGraphicsItemGroup *m_group;
};
