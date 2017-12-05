#pragma once
#include "preheader.h"
#include "sizehandle.h"
#define M_PI 3.14159265358979323846
static QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen)
{
    // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
    // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
    const qreal penWidthZero = qreal(0.00000001);

    if (path == QPainterPath() || pen == Qt::NoPen)
        return path;
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());

    ps.setWidth(pen.widthF());
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;

}

class GraphicsItem : public QAbstractGraphicsShapeItem
{
public:
    GraphicsItem(QGraphicsItem * parent );
    enum {Type = UserType+1};
    int  type() const { return Type; }
    SizeHandleRect::Direction  hitTest( const QPointF & point ) const;
    virtual void resizeTo(SizeHandleRect::Direction dir, const QPointF & point );
    virtual QPointF origin () const { return QPointF(0,0); }
    virtual Qt::CursorShape getCursor(SizeHandleRect::Direction dir );
    virtual QRectF  rect() const { return QRectF(0,0,0,0);}
    virtual void resetShape(){}
    void resetArrow()
    {
        if(m_rect.top()>m_rect.bottom())
        {
            qreal tmp =m_rect.bottom();
            m_rect.setBottom(m_rect.top());
            m_rect.setTop(tmp);
        }
        if(m_rect.left()>m_rect.right())
        {
            qreal tmp =m_rect.right();
            m_rect.setRight(m_rect.left());
            m_rect.setLeft(tmp);
        }
        updateGeometry();
    }


protected:
    virtual void updateGeometry();
    void setState(SelectionHandleState st);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event){}
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    typedef QVector<SizeHandleRect*> Handles;
    Handles m_handles;
    QRectF m_rect;
    QPen _pen;
    QColor _color =Qt::red;
};
