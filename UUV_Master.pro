QT += core gui widgets network

CONFIG += c++17

include ($$PWD/third_party/QJoysticks/QJoysticks.pri)

INCLUDEPATH += lib/inc

HEADERS += \
    lib/inc/Controller.h \
    lib/inc/JoysticksProxy.h \
    lib/inc/RemoterGuard.h \
    lib/inc/RobotData.h \
    lib/inc/RobotDataFormatter.h \
    lib/inc/RobotDataTransmitter.h \
    lib/inc/VideoWidget.h \
    lib/inc/VideoCoreThread.h \
    mainwindow.h

SOURCES += \
    lib/src/Controller.cpp \
    lib/src/JoysticksProxy.cpp \
    lib/src/RemoterGuard.cpp \
    lib/src/RobotData.cpp \
    lib/src/RobotDataFormatter.cpp \
    lib/src/RobotDataTransmitter.cpp \
    lib/src/VideoWidget.cpp \
    lib/src/VideoCoreThread.cpp \
    main.cpp \
    mainwindow.cpp

FORMS += \
    mainwindow.ui

DEFINES += MY_DEBUG

DESTDIR = $$PWD/bin
