#include "GraphicsPixmapItem.h"

GraphicsPixmapItem::GraphicsPixmapItem(const QPixmap &pixmap ,GraphicsItem *parent)
    :GraphicsItem(parent)
    ,m_pixmap(pixmap)
    ,m_width(pixmap.width())
    ,m_height(pixmap.height())
{
    m_rect =pixmap.rect();
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


QRectF GraphicsPixmapItem::boundingRect() const
{
    return rect();
}

QPainterPath GraphicsPixmapItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return qt_graphicsItem_shapeFromPath(path,_pen);
}

void GraphicsPixmapItem::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
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

    m_width = delta.width();
    m_height = delta.height();
    m_rect =delta;
    updateGeometry();
}



void GraphicsPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    qreal x =rect().left();
    if(rect().left() >rect().right())
        x =rect().right();
    qreal y =rect().top();
    if(rect().top() >rect().bottom())
        y =rect().bottom();
    painter->drawPixmap(x,y,m_pixmap.scaled(abs(rect().width()),abs(rect().height()),
                                                                   Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
}
