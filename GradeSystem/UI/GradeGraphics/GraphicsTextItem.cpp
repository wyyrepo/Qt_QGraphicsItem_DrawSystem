#include "GraphicsTextItem.h"


GraphicsTextItem::GraphicsTextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}
//! [0]

//! [1]
QVariant GraphicsTextItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
//    if (change == QGraphicsItem::ItemSelectedHasChanged)
//        emit selectedChange(this);
    return value;
}
//! [1]

//! [2]
void GraphicsTextItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
   // emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}
//! [2]

//! [5]
void GraphicsTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}
