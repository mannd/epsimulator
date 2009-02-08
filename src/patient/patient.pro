TEMPLATE = lib

DESTDIR = ../../lib/

SOURCES += heart.cpp \
saturation.cpp \
 patient.cpp
HEADERS += heart.h \
saturation.h \
 patient.h
INCLUDEPATH += ../core

#The following line was inserted by qt3to4
LIBS += -L../../lib \
 -lcore

QT -= gui

