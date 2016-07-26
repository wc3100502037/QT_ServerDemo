#-------------------------------------------------
#
# Project created by QtCreator 2016-03-03T15:02:52
#
#-------------------------------------------------

QT       += core gui
QT       +=network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyTcpServerDemoFrom
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clientthread.cpp \
    gprsclientthread.cpp


HEADERS  += mainwindow.h \
    clientthread.h \
    gprsclientthread.h


FORMS    += mainwindow.ui
