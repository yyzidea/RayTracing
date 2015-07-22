#-------------------------------------------------
#
# Project created by QtCreator 2015-07-14T00:39:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RayTracing
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    RayTracing/abberation.h \
    RayTracing/extra.h \
    RayTracing/fileIO.h \
    RayTracing/guass.h \
    RayTracing/headers.h \
    RayTracing/iniParameter.h \
    RayTracing/lighttrace.h \
    abberation.h \
    extra.h \
    fileIO.h \
    guass.h \
    headers.h \
    iniParameter.h \
    lighttrace.h

FORMS    += mainwindow.ui
