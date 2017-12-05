#include "MainPaintScenee.h"
#include<QGraphicsSceneMouseEvent>
#include<QGraphicsRectItem>
#include <QDebug>

const double initWidth =700.0;
const double initHeight =663.0;
MainPaintScene::MainPaintScene(QObject *parent) :
    QGraphicsScene(parent)
{
    //    QRect mainRect(0,0,2000,2000);
    //    setSceneRect(mainRect);//设置场景范围
    //    addRect(mainRect,QPen(Qt::black));//把场景范围直观显示出来，只为了方便理解

    //    _colorBak=Qt::red;

}

MainPaintScene::~MainPaintScene()
{
    SafeDelete (textItem);
}
void MainPaintScene::InsertPicture(QString path)
{
    DrawTool * tool =DrawTool::findTool(image);
    if (tool)
    {
        tool->setPicture(path);
    }
}

void MainPaintScene::mouseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    switch( mouseEvent->type() ){
    case QEvent::GraphicsSceneMousePress:
        QGraphicsScene::mousePressEvent(mouseEvent);
        break;
    case QEvent::GraphicsSceneMouseMove:
        QGraphicsScene::mouseMoveEvent(mouseEvent);
        break;
    case QEvent::GraphicsSceneMouseRelease:
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
        break;
    }
}


void MainPaintScene::SetBackGroundImage(QString path)
{
    this->clear();
    //this->addRect(0,0,initWidth,initHeight,QPen(Qt::green),QBrush(Qt::red));

    QPixmap pixmap =QPixmap::fromImage(QImage(path));
    QGraphicsPixmapItem* _backGroundItem= this->addPixmap(pixmap);

    if(pixmap.width()>pixmap.height())
    {
        //匹配宽度
        _backGroundItem->setScale( initWidth/pixmap.width() );
    }
    else//匹配高度
    {
        _backGroundItem->setScale( initHeight /pixmap.height() );
    }
    _backGroundItem->setPos(0,0);

}

void MainPaintScene::keyPressEvent(QKeyEvent *event)
{
    //    DrawTool * tool =DrawTool::findTool( DrawTool::c_drawShape );
    //    if (tool)
    //        tool->keyPressEvent(event,this);
    QList<QGraphicsItem *> selectItems =  selectedItems();
    switch (event->key())
    {
    case Qt::Key_Delete:
        if(selectItems.count()>0)
        {
            while(selectItems.count()>0)
            {
                QGraphicsItem * item =selectItems.last();
                removeItem(item);
                selectItems.pop_back();
            }
        }
        break;
    }


}

void MainPaintScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    DrawTool * tool =DrawTool::findTool( DrawTool::c_drawShape );
    if( DrawTool::c_drawShape  ==text) //文字独立出来
    {
        textItem = new GraphicsTextItem();

        QFont myFont =textItem->font();        
        myFont.setWeight(QFont::Bold);
        myFont.setFamily("Microsoft YaHei");
        myFont.setPixelSize(14);

        textItem->setDefaultTextColor(_color);
        textItem->setFont(myFont);
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        textItem->setZValue(1000.0);
        connect(textItem, SIGNAL(lostFocus(GraphicsTextItem*)),
                this, SLOT(editorLostFocus(GraphicsTextItem*)));
        connect(textItem, SIGNAL(selectedChange(GraphicsTextItem*)),
                this, SIGNAL(itemSelected(GraphicsTextItem*)));
        addItem(textItem);
        textItem->setDefaultTextColor(Qt::red);
        textItem->setPos(mouseEvent->scenePos());
        textItem->setFlag(QGraphicsItem::ItemIsMovable);
        textItem->setAcceptDrops(true);
        SetCurrentShape(selection);
    }
    else if (tool)
    {
        tool->mousePressEvent(mouseEvent,this);
    }

}
void MainPaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    DrawTool * tool = DrawTool::findTool( DrawTool::c_drawShape );
    if( DrawTool::c_drawShape  ==text) //文字独立出来
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
    else if(tool)
        tool->mouseMoveEvent(mouseEvent,this);


}

void MainPaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    DrawTool * tool =DrawTool::findTool( DrawTool::c_drawShape );
    if( DrawTool::c_drawShape  ==text) //文字独立出来
    {

    }
    else if ( tool )
        tool->mouseReleaseEvent(mouseEvent,this);
}
void MainPaintScene::setCurrentColor(QColor color)
{
    DrawTool * tool =DrawTool::findTool( DrawTool::c_drawShape );
    if ( tool )
        tool->setCurrentColor(color);
    _color =color;
}
void MainPaintScene::SetCurrentShape(DrawBaseShape shape)
{
    if ( shape == selection)
        DrawTool::c_drawShape = selection;
    if ( shape == rotation)
        DrawTool::c_drawShape = rotation;
    if ( shape == line)
        DrawTool::c_drawShape = line;
    if ( shape == rectangle)
        DrawTool::c_drawShape = rectangle;
    if ( shape == ellipse)
        DrawTool::c_drawShape = ellipse;
    if ( shape == penPath)
        DrawTool::c_drawShape = penPath;
    if ( shape == text)
        DrawTool::c_drawShape = text;
    if ( shape == image)
        DrawTool::c_drawShape = image;
    if ( shape == arrowLine)
        DrawTool::c_drawShape = arrowLine;
    if ( shape == triangle)
        DrawTool::c_drawShape = triangle;
    DrawTool * tool =DrawTool::findTool( DrawTool::c_drawShape );
    if ( tool )
        tool->setCurrentColor(_color);
}
