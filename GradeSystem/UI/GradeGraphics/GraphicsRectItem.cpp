#include "GraphicsRectItem.h"

GraphicsRectItem::GraphicsRectItem(const QRect & rect ,QGraphicsItem *parent)
    :GraphicsItem(parent)
    ,m_width(rect.width())
    ,m_height(rect.height())
    ,m_isSquare(false)
{

    m_rect =rect;
    m_handles.reserve(SizeHandleRect::None);
    for (int i = SizeHandleRect::LeftTop; i <= SizeHandleRect::Left; ++i) {
        SizeHandleRect *shr = new SizeHandleRect(this, static_cast<SizeHandleRect::Direction>(i), this);
        m_handles.push_back(shr);
    }
    updateGeometry();
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
}

QRectF GraphicsRectItem::boundingRect() const
{
    return rect();
}

QPainterPath GraphicsRectItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return qt_graphicsItem_shapeFromPath(path,_pen);
}

void GraphicsRectItem::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
{
    QPointF local = mapFromScene(point);
    QString dirName;

    QRect delta =m_rect.toRect();
    switch (dir) {
    case SizeHandleRect::LeftTop:
        delta.setTopLeft(local.toPoint());

        break;
    case SizeHandleRect::Top:
        dirName = "Top";
        delta.setTop(local.y());
        break;

    case SizeHandleRect::RightTop:
        dirName = "RightTop";
        delta.setTopRight(local.toPoint());
        break;

    case SizeHandleRect::Left:
        dirName = "Left";
        delta.setLeft(local.x());
        break;
    case SizeHandleRect::Right:
        dirName = "Rigth";
        delta.setRight(local.x());
        break;

    case SizeHandleRect::LeftBottom:
        dirName = "LeftBottom";
        delta.setBottomLeft(local.toPoint());
        break;

    case SizeHandleRect::Bottom:
        dirName = "Bottom";
        delta.setBottom(local.y());
        break;

    case SizeHandleRect::RightBottom:
        dirName = "RightBottom";
        delta.setBottomRight(local.toPoint());
        break;

    default:
        break;
    }
    prepareGeometryChange();
    if(m_isSquare)
    {
        delta.setHeight(delta.width());
    }
    m_width = delta.width();
    m_height = delta.height();
    m_rect =delta;
    updateGeometry();
}



void GraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen =painter->pen();
    pen.setWidth(4);
    pen.setColor(m_color);
    painter->setPen(pen);
    painter->drawRect(rect());
}
