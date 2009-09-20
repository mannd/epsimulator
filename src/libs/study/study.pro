include(../../epsimulatorlibrary.pri)

TEMPLATE = lib

TARGET = study

INCLUDEPATH += \
    ../core \
    ../patient \
    ../hardware

LIBS += \
    -L../../lib \
    -lcore \
    -lpatient \
    -lhardware

SOURCES += \
    study.cpp \
    studyconfiguration.cpp

HEADERS += \
    study.h \
    studyconfiguration.h

