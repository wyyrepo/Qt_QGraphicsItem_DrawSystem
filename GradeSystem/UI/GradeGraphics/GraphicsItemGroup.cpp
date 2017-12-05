#include "GraphicsItemGroup.h"


GraphicsItemGroup::GraphicsItemGroup(QGraphicsItemGroup *group, QGraphicsItem *parent)
    :GraphicsRectItem(QRect(0,0,0,0),parent )
    ,m_group(group)
{
    if ( m_group ){
        m_group->setParentItem(this);
        QRectF bound = m_group->boundingRect();

        qDebug()<<" group rect :" << m_group->boundingRect() << " new Rect:" << bound;
        bound.normalized();
        prepareGeometryChange();
        m_width = bound.width();
        m_height = bound.height();

        QPointF delta = pos() - bound.center() ;

        setTransform(transform().translate(delta.x(),delta.y()));
        setTransformOriginPoint( bound.center());
        moveBy(-delta.x(),-delta.y());

        m_group->setTransformOriginPoint(bound.center());

        qDebug()<<"pos :" << delta;
        updateGeometry();
    }
}

QRectF GraphicsItemGroup::boundingRect() const
{

    QRectF bounding = m_group->mapRectToParent(m_group->boundingRect());

    return bounding;
}

QPainterPath GraphicsItemGroup::shape() const
{
    return m_group->shape() ;
}

void GraphicsItemGroup::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
{

}

void GraphicsItemGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPointF origin = mapFromScene(pos());

    QPointF origin1 = m_group->transformOriginPoint();

    QColor c1 = QColor(Qt::blue);
  //  c1.setAlpha(180);
    painter->setBrush(c1);
    painter->drawEllipse(origin.x() - 3 , origin.y() - 3 ,6,6);


    QColor c2 = QColor(Qt::green);
  //  c2.setAlpha(180);
    painter->setBrush(c2);
    painter->drawEllipse(origin1.x() - 3 , origin1.y() - 3 ,6,6);

}
