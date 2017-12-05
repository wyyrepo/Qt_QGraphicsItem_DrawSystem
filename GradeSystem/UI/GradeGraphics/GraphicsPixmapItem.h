#pragma once
#include "preheader.h"
#include "GraphicsItem.h"
class GraphicsPixmapItem : public GraphicsItem
{
public:
    GraphicsPixmapItem(const QPixmap &pixmap ,GraphicsItem * parent);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void resetShape()
    {
        m_rect.setLeft(0);
        m_rect.setTop(0);
        m_rect.setWidth(abs(m_width));
        m_rect.setHeight(abs(m_height));
        updateGeometry();
    }
    virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF & point );
    virtual QRectF  rect() const {
        return
                QRectF( QPointF(m_rect.left(),m_rect.top()),
                        QPointF(m_rect.right(), m_rect.bottom())) ;}protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    qreal m_width;
    qreal m_height;
    QPixmap m_pixmap;
};
