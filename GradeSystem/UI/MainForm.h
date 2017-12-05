#pragma once
#include "preheader.h"
#include "GradeGraphics/PaintView.h"
#include "GradeGraphics/MainPaintScenee.h"

namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();


    void on_pushButton_5_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_Color_clicked();
    void onSetColor(QColor color);

    void on_pushButton_6_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_12_clicked();



    void on_pushButton_13_clicked();

    void on_pbt_line_clicked();

    void on_pbt_Rect_clicked();

    void on_pbt_triangle_clicked();

    void on_pbt_circle_clicked();

    void on_pbt_Blue_clicked();

    void on_pbt_Green_clicked();

    void on_pbt_prePage_clicked();

signals:
    void changeCurrentShape(DrawBaseShape shape);
private:
    Ui::MainForm *ui;
    MainPaintScene *_mainPaintScene;

    void initToolBar();//将所有该隐藏的按钮隐藏；
    void resizeEvent(QResizeEvent *event);
};

