INCLUDEPATH += $$PWD/ffmpeg4/include

HEADERS += $$PWD/ffmpeginclude.h
HEADERS += $$PWD/ffmpeg.h

SOURCES += $$PWD/ffmpeg.cpp

LIBS += -L$$PWD/ffmpeg4/libwin64/ -lavformat -lavfilter -lavcodec -lswresample -lswscale -lavutil
