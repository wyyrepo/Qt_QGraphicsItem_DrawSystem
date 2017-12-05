#ifndef PREHEADER_H
#define PREHEADER_H
#include <QtCore>
#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDialog>
#include <QEvent>

enum DrawBaseShape
{
    selection ,
    rotation  ,
    line ,
    rectangle ,  //矩形
    roundrect ,
    ellipse ,
    poly,
    arrowLine, //箭头线段
    text, //文字
    penPath, //铅笔
    image, //图片
    triangle, //三角形
    None
};

#define SafeDelete(p) if (nullptr != p) { delete p; p = nullptr; }
#define USERCATALOGDIR  QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
#endif // PREHEADER_H
