TEMPLATE = lib

CONFIG += dll \
debug
CONFIG -= release

DESTDIR = ../../lib/

SOURCES += heart.cpp \
study.cpp
HEADERS += heart.h \
study.h
INCLUDEPATH += ../epsim

