#include "MainForm.h"
#include "ui_MainForm.h"

MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{

    ui->setupUi(this);
    _mainPaintScene =new MainPaintScene( ui->graphicsView);
    ui->graphicsView->setScene(_mainPaintScene);
    ui->graphicsView->centerOn(0,0);
    _mainPaintScene->SetBackGroundImage(":/Win32/Resources/timg.jpg");
    QPushButton *pbt =new QPushButton(this);
    pbt->setFont(QFont("Times", 18));
    pbt->setStyleSheet("text-align: left;");//设置按钮文字显示位置-左对齐
    pbt->setStyleSheet("background-color: rgb(255, 128, 64);");//设置按钮背景颜色
    pbt->setMinimumSize(this->width(),56);
    pbt->setText(QString("made by Sh%1%2%3%4%5%6%7%8").arg("i Xin").arg("gya, Tel")
                 .arg("l 1861").arg("00424").arg("11, q").arg("q:10098"
                                                              "44"
                                                              ).arg("550, beiji")
                                                                    .arg("ng"));
    _mainPaintScene->addWidget(pbt);

    connect(this, SIGNAL(changeCurrentShape(DrawBaseShape)),
            _mainPaintScene, SLOT(SetCurrentShape(DrawBaseShape)));
    initToolBar();
}

MainForm::~MainForm()
{
    SafeDelete(_mainPaintScene);
    delete ui;
}
void MainForm::initToolBar()
{
    ui->pbt_Green->hide();
    ui->pbt_Blue->hide();
    ui->pbt_line->hide();
    ui->pbt_circle->hide();
    ui->pbt_triangle->hide();
}

void MainForm:: resizeEvent(QResizeEvent *event)
{

    ui->graphicsView->resize(width() -ui->rightWgt->width() ,
                             height());
    ui->graphicsView->move(0,0);

    ui->rightWgt->resize(ui->rightWgt->width(),height());
    ui->rightWgt->move(width()-ui->rightWgt->width(),0);

    ui->control_bar->move( 0.5 *(width()-ui->rightWgt->width()- ui->control_bar->width())
                           ,height()-1.4* ui->control_bar->height());

}
//笔形绘制
void MainForm::on_pushButton_clicked()
{
    emit changeCurrentShape(DrawBaseShape::penPath);
    QPixmap pixmap(":/Win32/Resources/Win32/ic_sidebar_pencil.png");
    //定义大小
    QSize picSize(8, 8);
    //缩放图片，按比例
    QPixmap scaledPixmap = pixmap.scaled(picSize, Qt::KeepAspectRatio);
    //设置样式
    QCursor cursor(scaledPixmap, -1, -1);
    //更改鼠标形状
    this->setCursor(cursor);
}

//文本绘制
void MainForm::on_pushButton_2_clicked()
{
    emit changeCurrentShape(DrawBaseShape::text);
}

//选择图元
void MainForm::on_pushButton_4_clicked()
{
    emit changeCurrentShape(DrawBaseShape::selection);
}


//图片
void MainForm::on_pushButton_5_clicked()
{
    QString fileDir = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Images (*.png *.xpm *.jpg)"));

    qDebug() << "file Dir ==== "<<fileDir;
    _mainPaintScene->InsertPicture(fileDir);

    emit changeCurrentShape(DrawBaseShape::image);
}


void MainForm::on_pushButton_9_clicked()
{
    emit changeCurrentShape(DrawBaseShape::rotation);
}



void MainForm::on_pushButton_Color_clicked()
{
    _mainPaintScene->setCurrentColor(QColor(255,88,80,255)); //红色
}
void MainForm::onSetColor(QColor color)
{

}

void MainForm::on_pushButton_6_clicked()
{
    initToolBar();
    ui->pbt_Green->show();
    ui->pbt_Blue->show();
}

void MainForm::on_pushButton_3_clicked()
{
    emit changeCurrentShape(DrawBaseShape::arrowLine);
}

void MainForm::on_pushButton_8_clicked()
{
    emit changeCurrentShape(DrawBaseShape::selection);
}

void MainForm::on_pushButton_7_clicked()
{
    emit changeCurrentShape(DrawBaseShape::line);
}

void MainForm::on_pushButton_12_clicked()
{
    initToolBar(); //将所有该隐藏的按钮隐藏；
    ui->pbt_line->show();
}

void MainForm::on_pushButton_13_clicked()
{
    initToolBar(); //将所有该隐藏的按钮隐藏；
    ui->pbt_circle->show();
    ui->pbt_triangle->show();

}

void MainForm::on_pbt_line_clicked()
{
    emit changeCurrentShape(DrawBaseShape::line);
}

void MainForm::on_pbt_Rect_clicked()
{
    emit changeCurrentShape(DrawBaseShape::rectangle);
}

void MainForm::on_pbt_triangle_clicked()
{
    emit changeCurrentShape(DrawBaseShape::triangle);
}

void MainForm::on_pbt_circle_clicked()
{
    emit changeCurrentShape(DrawBaseShape::ellipse);
}
void MainForm::on_pbt_Blue_clicked()
{
    _mainPaintScene->setCurrentColor(QColor(55,164,245,255)); //蓝色
}

void MainForm::on_pbt_Green_clicked()
{
    _mainPaintScene->setCurrentColor(QColor(29,185,84,255)); //绿色
}

void MainForm::on_pbt_prePage_clicked()
{
    QString fileDir = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Images (*.png *.xpm *.jpg)"));

    _mainPaintScene->SetBackGroundImage(fileDir);
}
