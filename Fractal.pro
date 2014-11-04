#-------------------------------------------------
#
# Project created by QtCreator 2014-10-21T09:26:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fractal
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    pixelcalculationthread.cpp \
    imagefragments.cpp \
    customscene.cpp \
    apptimer.cpp

HEADERS  += mainwindow.h \
    pixelcalculationthread.h \
    imagefragments.h \
    customscene.h \
    apptimer.h

FORMS    += mainwindow.ui
