#ifndef SIZEHANDLE
#define SIZEHANDLE

#include <QGraphicsRectItem>
#include <QList>

QT_BEGIN_NAMESPACE
class QFocusEvent;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE


enum { SELECTION_HANDLE_SIZE = 6, SELECTION_MARGIN = 10 };
enum SelectionHandleState { SelectionHandleOff, SelectionHandleInactive, SelectionHandleActive };

class SizeHandleRect :public QGraphicsRectItem
{
public:
    enum Direction { LeftTop , Top, RightTop, Right, RightBottom, Bottom, LeftBottom, Left , Center, None};

    SizeHandleRect(QGraphicsItem* parent , Direction d, QGraphicsItem *resizable);

    Direction dir() const  { return m_dir; }
    void updateCursor();
    void setState(SelectionHandleState st);
    bool hitTest( const QPointF & point );
    void move(qreal x, qreal y );

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    const Direction m_dir;
    QPoint m_startPos;
    QPoint m_curPos;
    QSize m_startSize;
    QSize m_curSize;
    QGraphicsItem *m_resizable;
    SelectionHandleState m_state;
};


#endif // SIZEHANDLE

