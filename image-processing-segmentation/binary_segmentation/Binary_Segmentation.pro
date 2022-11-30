QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ImageSegmentationUI.cpp \
    ImageSegmentor.cpp \
    main.cpp \
    MatToQImage.cpp

HEADERS += \
    ImageSegmentationUI.h \
    ImageSegmentor.h \
    MatToQImage.h

FORMS += \
    ImageSegmentationUI.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Change these paths to the opencv build directory
INCLUDEPATH += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\include
LIBS += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin\libopencv_core452.dll
LIBS += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin\libopencv_highgui452.dll
LIBS += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin\libopencv_imgcodecs452.dll
LIBS += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin\libopencv_imgproc452.dll
LIBS += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin\libopencv_features2d452.dll
LIBS += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin\libopencv_calib3d452.dll
LIBS += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin\libopencv_video452.dll
LIBS += D:\OpenCV-MinGWFINALE\OpenCV-MinGWFINALE\OpenCV-MinGW-Build-OpenCV-4.5.2-x64\x64\mingw\bin\libopencv_videoio452.dll

RESOURCES += \
    ImageSegmentationUI.qrc
