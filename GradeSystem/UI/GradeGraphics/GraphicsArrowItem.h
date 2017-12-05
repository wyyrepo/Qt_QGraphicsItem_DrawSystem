#pragma once
#include "preheader.h"
#include "GraphicsItem.h"
class GraphicsArrowItem : public QGraphicsItem
{
public:
    GraphicsArrowItem(const QPointF &start = QPointF(),
                      const QPointF &end = QPointF(),
                      QGraphicsItem * parent=0);
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *w) Q_DECL_OVERRIDE;
    QPointF startPos,endPos;
    void setBorderColor(QColor color)
    {
        _mColor =color;
    }
private:
    QPainterPath getArrowsPath(const QPointF &start,
                               const QPointF &end,
                               qreal len) const;


private:
    qreal   len;
    QColor _mColor;
};
