TEMPLATE = lib

CONFIG += staticlib

FORMS += ui/dial.ui \
         ui/basicstimulator.ui \
 ui/teststimulator.ui

DESTDIR = ../../lib/

RESOURCES += ../resources/resources.qrc

SOURCES += stimulator.cpp \
 teststimulator.cpp \
 basicstimulator.cpp

HEADERS += stimulator.h \
 teststimulator.h \
 basicstimulator.h

INCLUDEPATH += ../core

DISTFILES += .

