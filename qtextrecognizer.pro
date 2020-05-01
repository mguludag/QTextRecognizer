#-------------------------------------------------
#
# Project created by QtCreator 2020-04-08T12:14:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtextrecognizer
TEMPLATE = app

win32-*{
include(C:/opencv/opencv410.pri)
include(C:/tesseract/tesseract410.pri)
include(C:/leptonica/leptonica178.pri)
}
unix{
include(/home/cheytac/opencv/opencv410.pri)
include(/home/cheytac/tesseract/tesseract410.pri)
include(/home/cheytac/leptonica/leptonica178.pri)
}
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11


SOURCES += \
        aboutdialog.cpp \
        adjustwidget.cpp \
        convertimage.cpp \
        graphics_view_zoom.cpp \
        main.cpp \
        mainwindow.cpp \
        mygraphicsview.cpp \
        mypoint.cpp \
        mypolyf.cpp \
        myrectf.cpp \
        myscene.cpp \
        ocrwidget.cpp \
        preprocessimage.cpp \
        rotatewidget.cpp \
        settingswidget.cpp

HEADERS += \
        aboutdialog.hpp \
        adjustwidget.hpp \
        convertimage.h \
        graphics_view_zoom.h \
        mainwindow.hpp \
        mygraphicsview.hpp \
        mypoint.h \
        mypolyf.hpp \
        myrectf.hpp \
        myscene.hpp \
        ocrwidget.hpp \
        preprocessimage.h \
        rotatewidget.hpp \
        settings.h \
        settingswidget.hpp

FORMS += \
        aboutdialog.ui \
        adjustwidget.ui \
        mainwindow.ui \
        ocrwidget.ui \
        rotatewidget.ui \
        settingswidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
