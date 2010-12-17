include(../../epsimulatorlibrary.pri)
include(../../version.pri)
TEMPLATE = lib
TARGET = core
QT -= gui
SOURCES += options.cpp \
    user.cpp \
    fileutilities.cpp \
    passwordhandler.cpp \
    probability.cpp \
    interval.cpp \
    mark.cpp \
    columnformat.cpp
unix:SOURCES += ../../os/fileutilities_unix.cpp
win32:SOURCES += ../../os/fileutilities_win.cpp
HEADERS += options.h \
    error.h \
    user.h \
    fileutilities.h \
    passwordhandler.h \
    probability.h \
    epdefs.h \
    interval.h \
    mark.h \
    columnformat.h \
    coreconstants.h
win32:LIBS += -lws2_32 \
    -lkernel32 \
    -loleaut32
