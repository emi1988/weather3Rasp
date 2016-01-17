#-------------------------------------------------
#
# Project created by QtCreator 2015-12-26T11:00:11
#
#-------------------------------------------------

QT       += core gui
QT += core
QT += network widgets
QT += xml

QT += webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#greaterThan(QT_MAJOR_VERSION, 4): QT += webkitwidgets

TARGET = weather3pi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    interrupts.cpp

HEADERS  += mainwindow.h \
    defs.h \
    interrupts.h

FORMS    += mainwindow.ui

RESOURCES += \
    nightPicturesRes.qrc \
    dayPicturesRes.qrc \
    iconsRes.qrc
