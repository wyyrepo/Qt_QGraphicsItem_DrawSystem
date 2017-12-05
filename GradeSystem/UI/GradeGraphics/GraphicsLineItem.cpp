#include "GraphicsLineItem.h"

GraphicsLineItem::GraphicsLineItem(const QLineF &line,QGraphicsItem *parent)
    :QGraphicsLineItem(line,parent)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
}

