include(../../epsimulatorlibrary.pri)
TEMPLATE = lib
TARGET = core
QT -= gui
SOURCES += options.cpp \
    versioninfo.cpp \
    user.cpp \
    fileutilities.cpp \
    passwordhandler.cpp \
    probability.cpp \
    eplists.cpp \
    interval.cpp \
    mark.cpp
HEADERS += options.h \
    versioninfo.h \
    error.h \
    user.h \
    fileutilities.h \
    passwordhandler.h \
    probability.h \
    epdefs.h \
    eplists.h \
    interval.h \
    itemlist.h \
    mark.h

LIBS += -LC:\Qt\2010.01\mingw\lib -lws2_32
