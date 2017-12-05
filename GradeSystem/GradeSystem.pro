#-------------------------------------------------
#
# Project created by QtCreator 2017-11-13T14:33:55
#
#-------------------------------------------------

QT       += core gui network sql multimedia winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GradeSystem
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    UI/GradeGraphics/MainPaintScene.cpp \
    UI/GradeGraphics/PaintView.cpp \
    UI/MainForm.cpp \
    UI/GradeGraphics/drawtool.cpp \
    UI/GradeGraphics/sizehandle.cpp \
    UI/GradeGraphics/GraphicsTextItem.cpp \
    UI/GradeGraphics/GraphicsRectItem.cpp \
    UI/GradeGraphics/GraphicsItem.cpp \
    UI/GradeGraphics/GraphicsEllipseItem.cpp \
    UI/GradeGraphics/GraphicsLineItem.cpp \
    UI/GradeGraphics/GraphicsItemGroup.cpp \
    UI/GradeGraphics/GraphicsPixmapItem.cpp \
    UI/GradeGraphics/GraphicsPathItem.cpp \
    UI/GradeGraphics/GraphicsArrowItem.cpp \
    UI/GradeGraphics/ShapeMimeData.cpp \
    UI/GradeGraphics/GraphicsTriangleItem.cpp \
    main.cpp

HEADERS += \
    preheader.h \
    UI/GradeGraphics/MainPaintScenee.h \
    UI/GradeGraphics/PaintView.h \
    UI/MainForm.h \
    UI/GradeGraphics/drawtool.h \
    UI/GradeGraphics/sizehandle.h \
    UI/GradeGraphics/GraphicsTextItem.h \
    UI/GradeGraphics/GraphicsRectItem.h \
    UI/GradeGraphics/GraphicsItem.h \
    UI/GradeGraphics/GraphicsEllipseItem.h \
    UI/GradeGraphics/GraphicsLineItem.h \
    UI/GradeGraphics/GraphicsItemGroup.h \
    UI/GradeGraphics/GraphicsPixmapItem.h \
    UI/GradeGraphics/GraphicsPathItem.h \
    UI/GradeGraphics/GraphicsArrowItem.h \
    UI/GradeGraphics/ShapeMimeData.h \
    UI/GradeGraphics/GraphicsTriangleItem.h

FORMS += \        
    UI/MainForm.ui \

include(qtsingleapplication/qtsingleapplication.pri)
PRECOMPILED_HEADER = preheader.h
CONFIG+=PRECOMPILED_HEADER

TRANSLATIONS+=translation_zh.ts

RESOURCES += \
    qrc.qrc

DISTFILES += \
    UI/TitleBar.ui.txt


