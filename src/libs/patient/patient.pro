include(../../epsimulatorlibrary.pri)

TEMPLATE = lib

TARGET = patient

SOURCES += \
    heart.cpp \
    saturation.cpp \
    patient.cpp

HEADERS += \
    heart.h \
    saturation.h \
    patient.h

INCLUDEPATH += ../core

LIBS += \
   -lcore

QT -= gui

