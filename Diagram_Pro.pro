#-------------------------------------------------
#
# Project created by QtCreator 2017-08-23T11:36:41
#
#-------------------------------------------------

QT       += core gui
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Diagram_Pro
TEMPLATE = app


SOURCES += main.cpp\
        node.cpp \
    link.cpp \
    diagramwindow.cpp

HEADERS  += node.h \
    link.h \
    diagramwindow.h

RESOURCES += \
    images.qrc
