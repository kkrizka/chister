#-------------------------------------------------
#
# Project created by QtCreator 2017-05-10T16:02:14
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chister
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    serialconsole.cpp \
    SerialDevice.cpp \
    ECS02UI.cpp \
    ECS02.cpp \
    FrameGrabber.cpp

HEADERS  += mainwindow.h \
    serialconsole.h \
    SerialDevice.h \
    ECS02UI.h \
    ECS02.h \
    FrameGrabber.h

FORMS    += mainwindow.ui \
    serialconsole.ui \
    ECS02UI.ui

LIBS += "C:\Program Files (x86)\National Instruments\NI-IMAQ\Lib\MSVC\imaq.lib"

INCLUDEPATH += "C:\Program Files (x86)\National Instruments\NI-IMAQ\Include"
LIBS += "/LIBPATH:C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib"
