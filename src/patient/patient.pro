TEMPLATE = lib

CONFIG += dll \
debug
CONFIG -= release

DESTDIR = ../../lib/

SOURCES += heart.cpp \
study.cpp \
 saturation.cpp \
 patient.cpp
HEADERS += heart.h \
study.h \
 saturation.h \
 patient.h
INCLUDEPATH += ../core

#The following line was inserted by qt3to4
LIBS += -L../../lib \
 -lcore


QT -= gui

