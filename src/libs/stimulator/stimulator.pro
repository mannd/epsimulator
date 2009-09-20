include(../../epsimulatorlibrary.pri)

TEMPLATE = lib

TARGET = stimulator

FORMS += \
    ui/dial.ui \
    ui/basicstimulator.ui \
    ui/teststimulator.ui

SOURCES += \
    stimulator.cpp \
    teststimulator.cpp \
    basicstimulator.cpp

HEADERS += \
    stimulator.h \
    teststimulator.h \
    basicstimulator.h

INCLUDEPATH += ../core
