#pragma once
#include "preheader.h"

class ShapeMimeData :public QMimeData
{
    Q_OBJECT
public:
    ShapeMimeData( QList<QGraphicsItem * > items);
    ~ShapeMimeData();
    QList<QGraphicsItem *> items() const ;
private:
    QList<QGraphicsItem * > m_items;
};
