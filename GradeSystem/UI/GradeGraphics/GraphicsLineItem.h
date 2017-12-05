#pragma once
#include "preheader.h"
#include "GraphicsRectItem.h"

class GraphicsLineItem : public QGraphicsLineItem
{
public:
    GraphicsLineItem(const QLineF &line,QGraphicsItem * parent=0 );

};
