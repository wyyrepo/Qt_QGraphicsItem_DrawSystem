#pragma once
#include "preheader.h"
#include "GraphicsItem.h"
class GraphicsPathItem : public GraphicsItem
{

public:
    GraphicsPathItem(const QPainterPath &painterPath ,GraphicsItem * parent);

    void setPath(QPainterPath path_in)
    {
        prepareGeometryChange();
        //_path = path_in;
//        _boundingRect =_path.boundingRect();
        update();
    }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF & point );
    virtual QRectF  rect() const { return QRectF(-m_width / 2 , -m_height / 2 , m_width,m_height) ;}
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    qreal m_width;
    qreal m_height;
    mutable QRectF _boundingRect;
    QPainterPath _path;
};

