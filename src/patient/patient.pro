TEMPLATE = lib

DESTDIR = ../../lib/

SOURCES += heart.cpp \
saturation.cpp \
 patient.cpp
HEADERS += heart.h \
saturation.h \
 patient.h
INCLUDEPATH += ../core

LIBS += -L../../lib \
 -lcore

QT -= gui

