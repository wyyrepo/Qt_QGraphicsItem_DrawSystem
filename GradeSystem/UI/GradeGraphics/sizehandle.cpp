#include "sizehandle.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <qdebug.h>



SizeHandleRect::SizeHandleRect(QGraphicsItem* parent , Direction d, QGraphicsItem *resizable)
    :QGraphicsRectItem(0,0,SELECTION_HANDLE_SIZE,SELECTION_HANDLE_SIZE,parent)
    ,m_dir(d)
    ,m_resizable(resizable)
    ,m_state(SelectionHandleOff)
{
    setParentItem(parent);
    hide();
}

void SizeHandleRect::updateCursor()
{
    switch (m_dir) {
    case Right:
        setCursor(Qt::SizeHorCursor);
        return;
    case RightTop:
        setCursor(Qt::SizeBDiagCursor);
        return;
    case RightBottom:
        setCursor(Qt::SizeFDiagCursor);
        return;
    case LeftBottom:
        setCursor(Qt::SizeBDiagCursor);
        return;
    case Bottom:
        setCursor(Qt::SizeVerCursor);
        return;
    case LeftTop:
        setCursor(Qt::SizeFDiagCursor);
        return;
    case Left:
        setCursor(Qt::SizeHorCursor);
        return;
    case Top:
        setCursor(Qt::SizeVerCursor);
        return;
    default:
        break;
    }
    setCursor(Qt::ArrowCursor);
}


void SizeHandleRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QColor c = QColor("limegreen");
  //  c.setAlpha(200);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(c));

    painter->drawRect(rect());

}


void SizeHandleRect::setState(SelectionHandleState st)
{
    if (st == m_state)
        return;
    switch (st) {
    case SelectionHandleOff:
        hide();
        break;
    case SelectionHandleInactive:
    case SelectionHandleActive:
        show();
        break;
    }
    m_state = st;
}

bool SizeHandleRect::hitTest(const QPointF &point)
{
    QPointF pt = mapFromScene(point);
    return rect().contains(pt);
}

void SizeHandleRect::move(qreal x, qreal y)
{
    setPos(x,y);
}





