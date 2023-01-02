#-------------------------------------------------
#
# Project created by QtCreator 2019-04-23T10:32:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = motiondetect
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\include
LIBS += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin\libopencv_core452.dll
LIBS += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin\libopencv_highgui452.dll
LIBS += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin\libopencv_imgcodecs452.dll
LIBS += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin\libopencv_imgproc452.dll
LIBS += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin\libopencv_features2d452.dll
LIBS += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin\libopencv_calib3d452.dll
LIBS += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin\libopencv_video452.dll
LIBS += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin\libopencv_videoio452.dll
