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
    FrameGrabber.cpp \
    AnalysisProgram.cpp \
    ImageScanAnalysis.cpp \
    SwissHCCAnalysis.cpp \
    SwissHCC_LoadChipsForm.cpp \
    SwissHCC_InfoForm.cpp \
    AnalysisProgramGUI.cpp \
    SwissHCCAnalysisGUI.cpp \
    SwissHCC_CrossAlignForm.cpp \
    SwissHCC_ChipTestForm.cpp \
    QOpenCVHelpers.cxx \
    SwissHCC_ConfigureForm.cpp \
    MicroZedHCC.cpp \
    SwissHCC_SummaryForm.cpp

HEADERS  += mainwindow.h \
    serialconsole.h \
    SerialDevice.h \
    ECS02UI.h \
    ECS02.h \
    FrameGrabber.h \
    AnalysisProgram.h \
    ImageScanAnalysis.h \
    SwissHCCAnalysis.h \
    SwissHCC_LoadChipsForm.h \
    SwissHCC_InfoForm.h \
    AnalysisProgramGUI.h \
    SwissHCCAnalysisGUI.h \
    SwissHCC_CrossAlignForm.h \
    SwissHCC_ChipTestForm.h \
    QOpenCVHelpers.h \
    SwissHCC_ConfigureForm.h \
    MicroZedHCC.h \
    SwissHCC_SummaryForm.h

FORMS    += mainwindow.ui \
    serialconsole.ui \
    ECS02UI.ui \
    SwissHCC_LoadChipsForm.ui \
    SwissHCC_InfoForm.ui \
    SwissHCC_CrossAlignForm.ui \
    SwissHCC_ChipTestForm.ui \
    SwissHCC_ConfigureForm.ui \
    SwissHCC_SummaryForm.ui

LIBS += "/LIBPATH:C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib"

# NI library
LIBS += "C:\Program Files (x86)\National Instruments\NI-IMAQ\Lib\MSVC\imaq.lib"

INCLUDEPATH += "C:\Program Files (x86)\National Instruments\NI-IMAQ\Include"

# OpenCV library
INCLUDEPATH +=C:\OpenCV\3.2.0\build\vc2015-x86\install\include
LIBS += C:\OpenCV\3.2.0\build\vc2015-x86\install\x86\vc14\lib\opencv_core320d.lib
LIBS += C:\OpenCV\3.2.0\build\vc2015-x86\install\x86\vc14\lib\opencv_imgproc320d.lib
LIBS += C:\OpenCV\3.2.0\build\vc2015-x86\install\x86\vc14\lib\opencv_imgcodecs320d.lib

RESOURCES += \
    swisshccanalysis.qrc
