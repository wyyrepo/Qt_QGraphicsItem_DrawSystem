#include "drawtool.h"
#include "GraphicsEllipseItem.h"
#include "GraphicsTextItem.h"
#include "GraphicsLineItem.h"
#include "GraphicsTriangleItem.h"
#define PI 3.14

QList<DrawTool*> DrawTool::c_tools;
QPointF DrawTool::c_down;
QPointF DrawTool::c_last;
quint32 DrawTool::c_nDownFlags;

DrawBaseShape DrawTool::c_drawShape = selection;

static SelectTool selectTool;
static PenPathTool penPathTool(penPath);
static LineTool   lineTool(line);
static ImageTool   imageTool(image);
static RectTool   rectTool(rectangle);
static RectTool   triangleTool(triangle);
static RectTool   roundRectTool(roundrect);
static RectTool   ellipseTool(ellipse);
static RotationTool rotationTool;
static ArrowItem arrowTool(arrowLine);


enum SelectMode
{
    none,
    netSelect,
    move, //移动
    size, //改变大小
    rotate //反转
};
SelectMode selectMode = none;

SizeHandleRect::Direction nDragHandle = SizeHandleRect::None;

static void setCursor(QGraphicsScene * scene , const QCursor & cursor )
{
    QList<QGraphicsView*> views  = scene->views();
    if ( views.count() > 0 ){
        QGraphicsView * view = views.first();
        view->setCursor(cursor);
    }
}

DrawTool::DrawTool(DrawBaseShape shape)
{
    m_drawShape = shape ;
    c_tools.push_back(this);
}

void DrawTool::mousePressEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    c_down = event->scenePos();
    c_last = event->scenePos();
}

void DrawTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    c_last = event->scenePos();
}

void DrawTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    if (event->scenePos() == c_down)
        c_drawShape = selection;
    setCursor(scene,Qt::ArrowCursor);

}

DrawTool *DrawTool::findTool(DrawBaseShape drawShape)
{
    QList<DrawTool*>::const_iterator iter = c_tools.constBegin();
    for ( ; iter != c_tools.constEnd() ; ++iter ){
        if ((*iter)->m_drawShape == drawShape )
            return (*iter);
    }
    return 0;
}

SelectTool::SelectTool()
    :DrawTool(selection)
{
    m_lastSize.setHeight(0);
    m_lastSize.setWidth(0);
    dashRect = 0;
    dashLine = 0;
    m_hoverSizer = false;

}

void SelectTool::mousePressEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    DrawTool::mousePressEvent(event,scene);

    if (!m_hoverSizer)
        scene->mouseEvent(event);

    selectMode = none;
    QList<QGraphicsItem *> items = scene->selectedItems();
    GraphicsItem *item = 0;
    if ( items.count() == 1 )
    {
        item = qgraphicsitem_cast<GraphicsItem*>(items.first());

    }
    if ( item != 0 &&
         item->type() !=GraphicsItem::UserType+3){

        nDragHandle = item->hitTest(event->scenePos());
        if ( nDragHandle !=SizeHandleRect::None)
            selectMode = size;
        else
            selectMode =  move;
    }


    if( selectMode == none ){
        selectMode = netSelect;
        QList<QGraphicsView*> views  = scene->views();
        if ( views.count() > 0 ){
            QGraphicsView * view = views.first();
            view->setDragMode(QGraphicsView::RubberBandDrag);
        }
    }

    if ( selectMode == move && items.count() == 1 ){

        if (dashRect ){
            scene->removeItem(dashRect);
            delete dashRect;
            dashRect = 0;
        }
        if (dashLine ) {
            scene->removeItem(dashLine);
            delete dashLine;
            dashLine = 0;
        }

        item->setPos(item->mapToScene(item->boundingRect().topLeft()));
        item->resetShape();
        dashRect = new QGraphicsPathItem(item->shape());
        dashRect->setPen(Qt::DashLine);
        dashRect->setPos(item->pos());
        //dashRect->setTransformOriginPoint(item->transformOriginPoint());
        dashRect->setTransform(item->transform());
        dashRect->setRotation(item->rotation());
        scene->addItem(dashRect);

        dashLine = new QGraphicsLineItem();
        dashLine->setPos(item->pos());
        dashLine->setPen(Qt::DashLine);
        scene->addItem(dashLine);

    }
}

void SelectTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    DrawTool::mouseMoveEvent(event,scene);
    bool isGroup = false;
    QList<QGraphicsItem *> items = scene->selectedItems();
    GraphicsItem * item = 0;
    if ( items.count() == 1 )
    {
        item = qgraphicsitem_cast<GraphicsItem*>(items.first());
        if ( item != 0 ){
            if(item->type() ==GraphicsItem::UserType+3)
            {
                scene->mouseEvent(event);
                scene->update();
                return;
            }
            if ( nDragHandle != SizeHandleRect::None && selectMode == size ){
                item->resizeTo(nDragHandle,c_last);
            }
            else if(nDragHandle == SizeHandleRect::None && selectMode == selection ){

                SizeHandleRect::Direction handle = item->hitTest(event->scenePos());
                if ( handle != SizeHandleRect::None){
                    setCursor(scene,item->getCursor(handle));
                    m_hoverSizer = true;
                }else{
                    setCursor(scene,Qt::ArrowCursor);
                    m_hoverSizer = false;
                }
            }
        }

        QGraphicsItemGroup *item1 = qgraphicsitem_cast<QGraphicsItemGroup*>(items.first());
        if ( item1 != NULL ){
            isGroup = true;
        }
    }
    if ( selectMode == move ){
        if ( dashRect )
        {
            dashRect->setPos(c_last);
            dashLine->setLine(QLineF(dashLine->mapFromItem(item,item->boundingRect().topLeft()),
                                     dashLine->mapFromScene(c_last)));
        }
    }
    else if ((selectMode != size  && items.count() > 0) || isGroup )
    {
        scene->mouseEvent(event);
    }
    scene->update();
}

void SelectTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    DrawTool::mouseReleaseEvent(event,scene);

    QList<QGraphicsItem *> items = scene->selectedItems();
    if ( items.count() == 1 ){
        GraphicsItem * item = qgraphicsitem_cast<GraphicsItem*>(items.first());
        if ( item != 0 )
        {
            if(selectMode == move && c_last != c_down ){
                item->setPos(c_last);
            }
            item->resetArrow();
        }
    }
    if (selectMode == netSelect ){
        QList<QGraphicsView*> views  = scene->views();
        if ( views.count() > 0 ){
            QGraphicsView * view = views.first();
            view->setDragMode(QGraphicsView::NoDrag);
        }
    }

    if (dashRect ){
        scene->removeItem(dashRect);
        delete dashRect;
        dashRect = 0;

        scene->removeItem(dashLine);
        delete dashLine;
        dashLine = 0;
    }

    selectMode = none;
    nDragHandle = SizeHandleRect::None;
    m_hoverSizer = false;
    scene->mouseEvent(event);
}


RotationTool::RotationTool()
    :DrawTool(rotation)
{
    lastAngle == 0;
    dashRect = 0;
}

void RotationTool::mousePressEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    DrawTool::mousePressEvent(event,scene);
    QList<QGraphicsItem *> items = scene->selectedItems();
    if ( items.count() == 1 ){
        GraphicsItem * item = qgraphicsitem_cast<GraphicsItem*>(items.first());
        if ( item != 0 ){
            nDragHandle = item->hitTest(event->scenePos());
            if ( nDragHandle !=SizeHandleRect::None)
            {
                QPointF origin = item->mapToScene(item->boundingRect().center());

                qreal len_y = c_last.y() - origin.y();
                qreal len_x = c_last.x() - origin.x();

                qreal angle = atan2(len_y,len_x)*180/PI;

                lastAngle = angle;
                selectMode = rotate;

                if (dashRect ){
                    scene->removeItem(dashRect);
                    delete dashRect;
                    dashRect = 0;
                }

                dashRect = new QGraphicsPathItem(item->shape());
                dashRect->setPen(Qt::DashLine);
                dashRect->setPos(item->pos());
                dashRect->setTransformOriginPoint(item->transformOriginPoint());
                dashRect->setTransform(item->transform());
                dashRect->setRotation(item->rotation());
                scene->addItem(dashRect);

                setCursor(scene,QCursor(QPixmap(":/image/rotation.png")));
            }
            else{

                scene->clearSelection();
                c_drawShape = selection;
            }
        }
    }
}

void RotationTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    DrawTool::mouseMoveEvent(event,scene);

    QList<QGraphicsItem *> items = scene->selectedItems();
    if ( items.count() == 1 ){
        GraphicsItem * item = qgraphicsitem_cast<GraphicsItem*>(items.first());
        if ( item != 0  && nDragHandle !=SizeHandleRect::None && selectMode == rotate ){


            QPointF origin = item->mapToScene(item->boundingRect().center());

            qreal len_y = c_last.y() - origin.y();
            qreal len_x = c_last.x() - origin.x();

            qreal angle = atan2(len_y,len_x)*180/PI;

            angle = item->rotation() + int(angle - lastAngle) ;

            if ( angle > 360 )
                angle -= 360;

            if ( dashRect ){
                dashRect->setRotation( angle );
                scene->update();
            }

            setCursor(scene,QCursor(QPixmap(":/image/rotation.png")));
        }
        else if ( item )
        {
            SizeHandleRect::Direction handle = item->hitTest(event->scenePos());
            if ( handle != SizeHandleRect::None)
                setCursor(scene,QCursor(QPixmap(":/image/rotation.png")));
            else
                setCursor(scene,Qt::ArrowCursor);
        }
    }
}

void RotationTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    DrawTool::mouseReleaseEvent(event,scene);

    QList<QGraphicsItem *> items = scene->selectedItems();
    if ( items.count() == 1 ){
        GraphicsItem * item = qgraphicsitem_cast<GraphicsItem*>(items.first());
        if ( item != 0  && nDragHandle !=SizeHandleRect::None && selectMode == rotate ){

            QPointF origin = item->mapToScene(item->boundingRect().center());

            QPointF delta = c_last - origin ;
            qreal len_y = c_last.y() - origin.y();
            qreal len_x = c_last.x() - origin.x();
            qreal angle = atan2(len_y,len_x)*180/PI;
            angle = item->rotation() + int(angle - lastAngle) ;
            if ( angle > 360 )
                angle -= 360;
            item->setRotation( angle );
        }
    }

    setCursor(scene,Qt::ArrowCursor);
    selectMode = none;
    nDragHandle = SizeHandleRect::None;
    lastAngle = 0;
    if (dashRect ){
        scene->removeItem(dashRect);
        delete dashRect;
        dashRect = 0;
    }
    qDebug() << " RotationTool release:" <<event->scenePos();
}

/*************************************ImageTool*******************/
ImageTool::ImageTool(DrawBaseShape drawShape)
    :DrawTool(drawShape)
{

}
void ImageTool::setPicture(QString path )
{
    _imgPath =path;

}
void ImageTool::mousePressEvent(QGraphicsSceneMouseEvent * event , MainPaintScene * scene )
{
    DrawTool::mousePressEvent(event,scene);
    scene->clearSelection();
    QPixmap pixmap;
    bool ok= pixmap.load(_imgPath);
    if(! ok)
        return;
    switch ( c_drawShape ){
    case image:
        item = new GraphicsPixmapItem(pixmap,NULL);
        break;

    }
    if ( item == 0) return;
    item->setPos(event->scenePos());
    scene->addItem(item);
    item->setSelected(true);
    selectMode = size;
    nDragHandle = SizeHandleRect::RightBottom;
}
void ImageTool::mouseMoveEvent(QGraphicsSceneMouseEvent * event , MainPaintScene * scene )
{
    setCursor(scene,Qt::CrossCursor);
    selectTool.mouseMoveEvent(event,scene);
}
void ImageTool::mouseReleaseEvent(QGraphicsSceneMouseEvent * event , MainPaintScene * scene )
{
    if ( event->scenePos() == c_down ){
        if ( item != 0)
            scene->removeItem(item);
        selectTool.mousePressEvent(event,scene);
    }
    selectTool.mouseReleaseEvent(event,scene);
}



/*************************************RectTool*******************/
RectTool::RectTool(DrawBaseShape drawShape)
    :DrawTool(drawShape)
{
}

void RectTool::mousePressEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    DrawTool::mousePressEvent(event,scene);
    scene->clearSelection();

    bool isSquare =false;
    if (event->button() == Qt::LeftButton)
    {
        if (QApplication::keyboardModifiers()== Qt::ControlModifier)
        {
            isSquare =true;
        }
    }
    switch ( c_drawShape ){
    case rectangle:
        item = new GraphicsRectItem(QRect(0,0,0,0),NULL);
        item->setPen(Qt::NoPen);
        break;
    case ellipse:
        item = new GraphicsEllipseItem(QRect(0,0,0,0),NULL);
        item->setPen(Qt::NoPen);
        break;
    case triangle:
        item = new GraphicsTriangleItem(QRect(0,0,0,0),NULL);
        item->setPen(Qt::NoPen);
        break;
    }
    if ( item == 0) return;
    if(isSquare)
        item->setIsSquare();
    item->setBorderColor(_mColor);
    item->setPos(event->scenePos());
    scene->addItem(item);
    item->setSelected(true);
    selectMode = size;
    nDragHandle = SizeHandleRect::RightBottom;
}

void RectTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    setCursor(scene,Qt::CrossCursor);
    selectTool.mouseMoveEvent(event,scene);
}

void RectTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    if ( event->scenePos() == c_down ){
        if (item != 0)
            scene->removeItem(item);
        selectTool.mousePressEvent(event,scene);
    }
    selectTool.mouseReleaseEvent(event,scene);
}

/////////****************PenPath************/////////////////////
PenPathTool::PenPathTool(DrawBaseShape drawShape)
    :DrawTool(drawShape)
{

    _path =nullptr;
}
void PenPathTool::mousePressEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    DrawTool::mousePressEvent(event,scene);
    scene->clearSelection();

    switch ( c_drawShape ){
    case penPath:
        SafeDelete(_path);
        _path = new QPainterPath;
        _path->moveTo(event->scenePos());
        item = new QGraphicsPathItem(*_path,nullptr);
        item->setFlag(QGraphicsItem::ItemIsMovable, true);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        break;
    }
    if ( item == 0) return;

    isDrawing =true;
    scene->addItem(item);
    QPen pen_bak=item->pen();
    pen_bak.setWidth(9);
    pen_bak.setColor(_mColor);
    item->setPen(pen_bak);

}

void PenPathTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    if(isDrawing)
    {
        QPixmap pixmap(":/Win32/Resources/Win32/ic_sidebar_pencil.png");
        //定义大小
        QSize picSize(8, 8);
        //缩放图片，按比例
        QPixmap scaledPixmap = pixmap.scaled(picSize, Qt::KeepAspectRatio);
        //设置样式
        QCursor cursor(scaledPixmap, -1, -1);
        setCursor(scene,cursor);
        _path->lineTo(event->scenePos());
        item->setPath(*_path);
    }
}

void PenPathTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    isDrawing=false;
}

/////////****************LineTool************/////////////////////
LineTool::LineTool(DrawBaseShape drawShape)
    :DrawTool(drawShape)
{

    item =nullptr;
}
void LineTool::mousePressEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    DrawTool::mousePressEvent(event,scene);
    scene->clearSelection();

    switch ( c_drawShape ){
    case line:
        item = new GraphicsLineItem(QLineF(event->scenePos(),
                                           event->scenePos()));
        item->setFlag(QGraphicsItem::ItemIsMovable, true);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        break;
    }
    if ( item == 0) return;

    isDrawing =true;
    // 设置画笔
    QPen pen = item->pen();
    pen.setColor(_mColor);
    pen.setWidth(4);
    item->setPen(pen);
    scene->addItem(item);
}

void LineTool::mouseMoveEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    if(isDrawing)
    {
        QLineF newLine(item->line().p1(), event->scenePos());
        item->setLine(newLine);
    }
}

void LineTool::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    isDrawing=false;
}
/////////****************ArrowItem************/////////////////////

ArrowItem::ArrowItem(DrawBaseShape drawShape)
    :DrawTool(drawShape)
{
}

void ArrowItem::mousePressEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    DrawTool::mousePressEvent(event,scene);
    scene->clearSelection();
    switch ( c_drawShape ){
    case arrowLine:
        item = new GraphicsArrowItem();
        item->setFlag(QGraphicsItem::ItemIsMovable, true);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);

        item->endPos= event->scenePos();
        item->startPos= event->scenePos();
        break;
    }
    if ( item == 0) return;
    isDrawing =true;
    item->setBorderColor(_mColor);
    scene->addItem(item);
}

void ArrowItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
    if(isDrawing)
    {
         item->endPos= event->scenePos();
         scene->update();
    }
}

void ArrowItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event, MainPaintScene *scene)
{
   isDrawing=false;
}

