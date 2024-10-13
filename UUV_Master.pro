QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include ($$PWD/third_party/QJoysticks/QJoysticks.pri)
include ($$PWD/third_party/ffmpeg/ffmpeg.pri)

HEADERS += \
    lib/inc/Controller.h \
    lib/inc/JoysticksProxy.h \
    lib/inc/RemoterGuard.h \
    lib/inc/RobotData.h \
    lib/inc/RobotDataFormatter.h \
    lib/inc/RobotDataTransmitter.h \
    mainwindow.h

SOURCES += \
    lib/src/Controller.cpp \
    lib/src/JoysticksProxy.cpp \
    lib/src/RemoterGuard.cpp \
    lib/src/RobotData.cpp \
    lib/src/RobotDataFormatter.cpp \
    lib/src/RobotDataTransmitter.cpp \
    main.cpp \
    mainwindow.cpp

FORMS += \
    mainwindow.ui

DEFINES += MY_DEBUG

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
