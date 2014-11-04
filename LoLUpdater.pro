#-------------------------------------------------
#
# Project created by QtCreator 2014-11-04T14:56:09
#
#-------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LoLUpdater
TEMPLATE = app
SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    ../LoLUpdater.h \
    resource.h

FORMS    += mainwindow.ui

RC_FILE = resource.rc

OTHER_FILES += \
    icon1.ico
