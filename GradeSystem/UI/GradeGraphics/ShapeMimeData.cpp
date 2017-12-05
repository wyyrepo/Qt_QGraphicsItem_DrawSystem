#include "ShapeMimeData.h"

ShapeMimeData::ShapeMimeData(QList<QGraphicsItem *> items)
{
    foreach (QGraphicsItem *item , items ) {
//       AbstractShape *sp = qgraphicsitem_cast<AbstractShape*>(item);
//       m_items.append(sp->copy());
    }
}
ShapeMimeData::~ShapeMimeData()
{
    foreach (QGraphicsItem *item , m_items ) {
        delete item;
    }
    m_items.clear();
}
