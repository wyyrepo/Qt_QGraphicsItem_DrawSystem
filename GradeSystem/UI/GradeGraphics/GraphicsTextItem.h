#pragma once
#include "preheader.h"

class GraphicsTextItem : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 3 };

    GraphicsTextItem(QGraphicsItem *parent = 0);

    int type() const override { return Type; }

signals:
    void lostFocus(GraphicsTextItem *item);
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void focusOutEvent(QFocusEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

};
