include(../../epsimulatorlibrary.pri)

TEMPLATE = lib

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
