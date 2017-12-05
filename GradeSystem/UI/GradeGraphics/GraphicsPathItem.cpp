#include "GraphicsPathItem.h"

GraphicsPathItem::GraphicsPathItem(const QPainterPath &path ,GraphicsItem *parent) :
    GraphicsItem(parent)
  ,_path(path)
  ,m_width(_path.boundingRect().width())
  ,m_height(_path.boundingRect().height())

{
    m_handles.reserve(SizeHandleRect::None);
    for (int i = SizeHandleRect::LeftTop; i <= SizeHandleRect::Left; ++i) {
        SizeHandleRect *shr = new SizeHandleRect(this, static_cast<SizeHandleRect::Direction>(i), this);
        m_handles.push_back(shr);
    }
    updateGeometry();
    _boundingRect =_path.controlPointRect();
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    this->setAcceptHoverEvents(true);
}

QRectF GraphicsPathItem::boundingRect() const
{
    if (_boundingRect.isNull()) {
        qreal pw = _pen.style() == Qt::NoPen ? qreal(0) : _pen.widthF();
        if (pw == 0.0)
            _boundingRect = _path.controlPointRect();
        else {
           _boundingRect = _path.controlPointRect();
        }
    }
    return _boundingRect;
}

QPainterPath GraphicsPathItem::shape() const
{
    QPainterPath path;
    path.addRect(_boundingRect);
    return qt_graphicsItem_shapeFromPath(path,_pen);
}

void GraphicsPathItem::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
{


    QPointF local = mapFromScene(point);
    QString dirName;

    const QRectF &geom = this->boundingRect();
    QRect delta = this->rect().toRect();
    switch (dir) {
    case SizeHandleRect::Right:
        dirName = "Rigth";
        delta.setRight(local.x());
        break;
    case SizeHandleRect::RightTop:
        dirName = "RightTop";
        delta.setTopRight(local.toPoint());
        break;
    case SizeHandleRect::RightBottom:
        dirName = "RightBottom";
        delta.setBottomRight(local.toPoint());
        break;
    case SizeHandleRect::LeftBottom:
        dirName = "LeftBottom";
        delta.setBottomLeft(local.toPoint());
        break;
    case SizeHandleRect::Bottom:
        dirName = "Bottom";
        delta.setBottom(local.y());
        break;
    case SizeHandleRect::LeftTop:
        dirName = "LeftTop";
        delta.setTopLeft(local.toPoint());
        break;
    case SizeHandleRect::Left:
        dirName = "Left";
        delta.setLeft(local.x());
        break;
    case SizeHandleRect::Top:
        dirName = "Top";
        delta.setTop(local.y());
        break;
    default:
        break;
    }

    prepareGeometryChange();
    m_width = qAbs(delta.width()) ;
    m_height = qAbs(delta.height());
    updateGeometry();
}



void GraphicsPathItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    QColor c = QColor(Qt::red);
    c.setAlpha(160);
    QPen _pen =_pen;
    _pen.setWidth(4);
    _pen.setColor(c);
    painter->setPen(_pen);

    painter->drawPath(_path);
}
