#include "GraphicsItem.h"
GraphicsItem::GraphicsItem(QGraphicsItem *parent)
    :QAbstractGraphicsShapeItem(parent)
{
    // QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    // effect->setBlurRadius(8);
    // setGraphicsEffect(effect);
}

void GraphicsItem::updateGeometry()
{
    const QRectF &geom = this->boundingRect();

    const int w = SELECTION_HANDLE_SIZE;
    const int h = SELECTION_HANDLE_SIZE;

    const Handles::iterator hend =  m_handles.end();
    for (Handles::iterator it = m_handles.begin(); it != hend; ++it) {
        SizeHandleRect *hndl = *it;;
        switch (hndl->dir()) {
        case SizeHandleRect::LeftTop:
            hndl->move(geom.x() - w / 2, geom.y() - h / 2);
            break;
        case SizeHandleRect::Top:
            hndl->move(geom.x() + geom.width() / 2 - w / 2, geom.y() - h / 2);
            break;
        case SizeHandleRect::RightTop:
            hndl->move(geom.x() + geom.width() - w / 2, geom.y() - h / 2);
            break;
        case SizeHandleRect::Right:
            hndl->move(geom.x() + geom.width() - w / 2, geom.y() + geom.height() / 2 - h / 2);
            break;
        case SizeHandleRect::RightBottom:
            hndl->move(geom.x() + geom.width() - w / 2, geom.y() + geom.height() - h / 2);
            break;
        case SizeHandleRect::Bottom:
            hndl->move(geom.x() + geom.width() / 2 - w / 2, geom.y() + geom.height() - h / 2);
            break;
        case SizeHandleRect::LeftBottom:
            hndl->move(geom.x() - w / 2, geom.y() + geom.height() - h / 2);
            break;
        case SizeHandleRect::Left:
            hndl->move(geom.x() - w / 2, geom.y() + geom.height() / 2 - h / 2);
            break;
        case SizeHandleRect::Center:
            hndl->move(geom.center().x()  - w / 2 , geom.center().y() - h / 2);
            break;
        default:
            break;
        }
    }

}

void GraphicsItem::setState(SelectionHandleState st)
{
    const Handles::iterator hend =  m_handles.end();
    for (Handles::iterator it = m_handles.begin(); it != hend; ++it)
        (*it)->setState(st);
}

SizeHandleRect::Direction GraphicsItem::hitTest(const QPointF &point) const
{
    const Handles::const_iterator hend =  m_handles.end();
    for (Handles::const_iterator it = m_handles.begin(); it != hend; ++it)
    {
        if ((*it)->hitTest(point) ){
            return (*it)->dir();
        }
    }
    return SizeHandleRect::None;
}

Qt::CursorShape GraphicsItem::getCursor(SizeHandleRect::Direction dir)
{
    switch (dir) {
    case SizeHandleRect::Right:
        return Qt::SizeHorCursor;
    case SizeHandleRect::RightTop:
        return Qt::SizeBDiagCursor;
    case SizeHandleRect::RightBottom:
        return Qt::SizeFDiagCursor;
    case SizeHandleRect::LeftBottom:
        return Qt::SizeBDiagCursor;
    case SizeHandleRect::Bottom:
        return Qt::SizeVerCursor;
    case SizeHandleRect::LeftTop:
        return Qt::SizeFDiagCursor;
    case SizeHandleRect::Left:
        return Qt::SizeHorCursor;
    case SizeHandleRect::Top:
        return Qt::SizeVerCursor;
    default:
        break;
    }
    return Qt::ArrowCursor;
}

void GraphicsItem::resizeTo(SizeHandleRect::Direction dir, const QPointF &point)
{

}

QVariant GraphicsItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if ( change == QGraphicsItem::ItemSelectedHasChanged ) {
        qDebug()<<" Item Selected : " << value.toString();
        setState(value.toBool() ? SelectionHandleActive : SelectionHandleOff);
    }else if ( change == QGraphicsItem::ItemRotationHasChanged ){
        qDebug()<<"Item Rotation Changed:" << value.toString();
    }else if ( change == QGraphicsItem::ItemTransformOriginPointHasChanged ){
        qDebug()<<"ItemTransformOriginPointHasChanged:" << value.toPointF();
    }
    return value;
}
