TEMPLATE = lib

CONFIG += dll \
debug
CONFIG -= release

DESTDIR = ../../lib/

SOURCES += heart.cpp \
study.cpp
HEADERS += heart.h \
study.h
INCLUDEPATH += ../core

#The following line was inserted by qt3to4
LIBS += -L../../lib \
 -lcore

TARGETDEPS += ../../lib/libcore.so

QT -= gui

