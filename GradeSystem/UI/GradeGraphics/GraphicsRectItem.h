#pragma once
#include "preheader.h"
#include "GraphicsItem.h"
class GraphicsRectItem : public GraphicsItem
{
public:
    GraphicsRectItem(const QRect & rect ,QGraphicsItem * parent);

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

    void setBorderColor(QColor color)
    {
        m_color =color;
    }
    void setIsSquare()
    {
        m_isSquare =true;
    }
    virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF & point );
    virtual QRectF  rect() const {
        return
                QRectF( QPointF(m_rect.left(),m_rect.top()),
                        QPointF(m_rect.right(), m_rect.bottom())) ;}

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    qreal m_width;
    qreal m_height;
    bool m_isSquare; //是否为正方形
    QColor m_color;



};
