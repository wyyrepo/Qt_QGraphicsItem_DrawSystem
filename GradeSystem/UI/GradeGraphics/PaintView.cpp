#include "PaintView.h"
#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH  viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()
PaintView::PaintView(QWidget *parent) :
    QGraphicsView(parent)
{
    _zoomDelta=0.1;  // 缩放的增量
    _scaleValue=1.0;
    _bMouseTranslate=false;
    _translateSpeed=1.0;

    this->setRenderHint(QPainter::Antialiasing);


}

PaintView::~PaintView()
{

}
void PaintView::ZoomIn()
{
    zoom(1 + _zoomDelta);
}

// 缩小
void PaintView::ZoomOut()
{
    zoom(1 - _zoomDelta);
}
void PaintView::zoom(float scaleFactor)
{
    // 防止过小或过大
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
    _scaleValue *= scaleFactor;
}

void PaintView::wheelEvent(QWheelEvent *event)
{
    // 滚轮的滚动量
    QPoint scrollAmount = event->angleDelta();
    // 正值表示滚轮远离使用者（放大），负值表示朝向使用者（缩小）
    scrollAmount.y() > 0 ? ZoomIn() :ZoomOut();
}

void  PaintView::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(this->viewport());
    painter.setBrush(Qt::gray);
    QFont font;
    font.setPixelSize(45);
    painter.setFont(font);
    //painter.drawb(QRectF(width()*0.3,height()*0.2,width()*0.3,height()*0.3),"made by andy!");

}
// 平移
void PaintView::mouseMoveEvent(QMouseEvent *event)
{
    if (_bMouseTranslate){
        QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(_lastMousePos);
        translate(mouseDelta);
    }

    _lastMousePos = event->pos();

    QGraphicsView::mouseMoveEvent(event);
}

void PaintView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == _translateButton) {
        // 当光标底下没有 item 时，才能移动
        QPointF point = mapToScene(event->pos());
        if (scene()->itemAt(point, transform()) == NULL)  {
            _bMouseTranslate = true;
            _lastMousePos = event->pos();
        }
    }

    QGraphicsView::mousePressEvent(event);
}

void PaintView::onAssemblageAction(bool)
{
    QList<QGraphicsItem *>  selectItems=this->scene()->selectedItems();
    QGraphicsItemGroup * groupItems =this->scene()->createItemGroup(selectItems);
    QRectF rect =groupItems->boundingRect();

    QGraphicsScene  *scene =new QGraphicsScene();
    scene->setSceneRect(rect);
    for(int i=0; i<selectItems.count(); i++)
    {
        QGraphicsItem *sp = qgraphicsitem_cast<QGraphicsItem*>(selectItems[i]);
        scene->addItem(sp);
    }
    scene->clearSelection();
    QImage image(rect.width(),rect.height(),QImage::Format_ARGB32);
    image.fill(Qt::transparent);//用透明色填充
    QString pngName = "D:/scene44test.png";
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    scene->render(&painter);
    bool saveSuccess =  image.save(pngName);
    
    for(int i=0; i<selectItems.count(); i++)
    {
        QGraphicsItem *sp = qgraphicsitem_cast<QGraphicsItem*>(selectItems[i]);
        this->scene()->addItem(sp);
    }
    if (!saveSuccess)
    {
        QMessageBox::about(NULL,"","组合图元失败!");
    }
    else
        QMessageBox::about(NULL,"","组合图元成功!");

    SafeDelete(scene);
}
void PaintView::onRotation1Action(bool)
{
    QList< QGraphicsItem*>  selectItems=this->scene()->selectedItems();
    int selectItemsCount =selectItems.count();

    for(int i=0; i<selectItemsCount ;i++)
    {
        //图元自转
        QPointF centerPos=  selectItems[i]->boundingRect().center() ;
        selectItems[i]->setTransformOriginPoint(centerPos);
        selectItems[i]->setRotation(selectItems[i]->rotation()+30);
    }
}
void PaintView::onRotation2Action(bool)
{
    QList<QGraphicsItem *>  selectItems=this->scene()->selectedItems();
    int selectItemsCount =selectItems.count();

    for(int i=0; i<selectItemsCount ;i++)
    {
        //图元自转
        QPointF centerPos=  selectItems[i]->boundingRect().center() ;
        selectItems[i]->setTransformOriginPoint(centerPos);
        selectItems[i]->setRotation(selectItems[i]->rotation()-30);
    }
}
void PaintView::onDeleteAction(bool)
{
    QList<QGraphicsItem *>  selectItems=this->scene()->selectedItems();
    int selectItemsCount =selectItems.count();


    for(int i=0; i<selectItemsCount ;i++)
    {
        this->scene()->removeItem(selectItems[i]);
    }
}
void PaintView::contextMenuEvent(QContextMenuEvent *event)
{
    QList<QGraphicsItem *>  selectItems=this->scene()->selectedItems();
    int selectItemsCount =selectItems.count();
    QMenu menu;
    QMimeData * mp = const_cast<QMimeData *>(QApplication::clipboard()->mimeData()) ;
    ShapeMimeData * data = dynamic_cast< ShapeMimeData*>( mp );
    if ( data ){
        QAction * pasteAction = menu.addAction("粘贴图元");
        connect(pasteAction,SIGNAL(triggered()),this,SLOT(on_paste));
    }
    if(selectItemsCount >0)
    {
        QAction * copyAction = menu.addAction("复制图元(+30)");
        connect(copyAction,SIGNAL(triggered()),this,SLOT(on_copy));
        QAction * cutAction = menu.addAction("剪切图元");
        connect(cutAction,SIGNAL(triggered()),this,SLOT(on_cut));
        if(selectItemsCount >1)
        {
            QAction *assemblageAction = menu.addAction("组合图元");
            connect(assemblageAction, SIGNAL(triggered(bool)),
                    this,SLOT(onAssemblageAction(bool)));
        }
        QAction * rotation1Action = menu.addAction("旋转图元(+30)");
        connect(rotation1Action, SIGNAL(triggered(bool)),
                this,SLOT(onRotation1Action(bool)));
        QAction *rotation2Action = menu.addAction("旋转图元(-30)");
        connect(rotation2Action, SIGNAL(triggered(bool)),
                this,SLOT(onRotation2Action(bool)));
        QAction *deleteAction = menu.addAction("删除图元");
        connect(deleteAction, SIGNAL(triggered(bool)),
                this,SLOT(onDeleteAction(bool)));
    }

    menu.exec(event->globalPos());

}

void PaintView::on_copy()
{

    ShapeMimeData * data = new ShapeMimeData(  this->scene()->selectedItems() );
    QApplication::clipboard()->setMimeData(data);
}

void PaintView::on_paste()
{

}

void PaintView::on_cut()
{

}
void PaintView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == _translateButton)
        _bMouseTranslate = false;

    QGraphicsView::mouseReleaseEvent(event);
}

void PaintView::resizeEvent(QResizeEvent *event)
{
  this->scene()->setSceneRect(this->rect());
}
void PaintView::translate(QPointF delta)
{
    // 根据当前 zoom 缩放平移数
    delta *= _scaleValue;
    delta *= _translateSpeed;

    // view 根据鼠标下的点作为锚点来定位 scene
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    QPoint newCenter(VIEW_WIDTH / 2 - delta.x(),  VIEW_HEIGHT / 2 - delta.y());
    centerOn(mapToScene(newCenter));

    // scene 在 view 的中心点作为锚点
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}


void PaintView::keyPressEvent(QKeyEvent *event)
{
    QList<QGraphicsItem *>  selectItems=this->scene()->selectedItems();
    int selectItemsCount =selectItems.count();
    switch (event->key()) {

    case Qt::Key_Up:
        if(selectItemsCount==0)
            translate(QPointF(0, -2));  // 上移
        else
        {
            for(int i=0; i<selectItemsCount ;i++)
            {
                selectItems[i]->setPos(selectItems[i]->pos().rx(),selectItems[i]->pos().ry()-1);
            }
        }

        break;
    case Qt::Key_Down:
        if(selectItemsCount==0)
            translate(QPointF(0, 2));  // 下移
        else
        {
            for(int i=0; i<selectItemsCount ;i++)
            {
                selectItems[i]->setPos(selectItems[i]->pos().rx(),selectItems[i]->pos().ry()+1);
            }
        }

        break;
    case Qt::Key_Left:
        if(selectItemsCount==0)
            translate(QPointF(-2, 0));  // 左移
        else
        {
            for(int i=0; i<selectItemsCount ;i++)
            {
                selectItems[i]->setPos(selectItems[i]->pos().rx()-1,selectItems[i]->pos().ry());
            }
        }
        break;
    case Qt::Key_Right:
        if(selectItemsCount==0)
            translate(QPointF(2, 0));  // 左移
        else
        {
            for(int i=0; i<selectItemsCount ;i++)
            {
                selectItems[i]->setPos(selectItems[i]->pos().rx()+1,selectItems[i]->pos().ry());
            }
        }
        break;
    case Qt::Key_Plus:  // 放大
        if(selectItemsCount==0)
            ZoomIn();
        else
        {
            for(int i=0; i<selectItemsCount ;i++)
            {
                qreal scaleSize=selectItems[i]->scale()+0.2;
                if(scaleSize>20) scaleSize=20;
                QPointF centerPos=  selectItems[i]->boundingRect().center() ;
                selectItems[i]->setTransformOriginPoint(centerPos);
                selectItems[i]->setScale(scaleSize);
            }
        }
        break;
    case Qt::Key_Minus:  // 缩小
        if(selectItemsCount==0)
            ZoomOut();
        else
        {
            for(int i=0; i<selectItemsCount ;i++)
            {
                qreal scaleSize=selectItems[i]->scale()-0.2;
                if(scaleSize<0.05) scaleSize=0.05;
                QPointF centerPos=  selectItems[i]->boundingRect().center() ;
                selectItems[i]->setTransformOriginPoint(centerPos);
                selectItems[i]->setScale(scaleSize);
            }
        }
        break;
    case Qt::Key_Space:  // 逆时针旋转
        if(selectItemsCount==0)
            rotate(-90);
        else
        {
            for(int i=0; i<selectItemsCount ;i++)
            {
                //图元自转
                QPointF centerPos=  selectItems[i]->boundingRect().center() ;
                selectItems[i]->setTransformOriginPoint(centerPos);
                selectItems[i]->setRotation(selectItems[i]->rotation()-1);
            }
        }
        break;
    case Qt::Key_Enter:  // 顺时针旋转
    case Qt::Key_Return:
        if(selectItemsCount==0)
            rotate(90);
        else
        {
            for(int i=0; i<selectItemsCount ;i++)
            {
                //图元自转
                QPointF centerPos=  selectItems[i]->boundingRect().center() ;
                selectItems[i]->setTransformOriginPoint(centerPos);
                selectItems[i]->setRotation(selectItems[i]->rotation()+1);
            }
        }
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}
void PaintView::SetTranslateSpeed(qreal speed)
{
    // 建议速度范围
    Q_ASSERT_X(speed >= 0.0 && speed <= 5.0,
               "InteractiveView::setTranslateSpeed", "Speed should be in range [0.0,5.0].");
    _translateSpeed = speed;
}
