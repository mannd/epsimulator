TEMPLATE = lib

CONFIG += staticlib \
 debug

FORMS += ui/dial.ui \
         ui/basicstimulator.ui \
 ui/teststimulator.ui


CONFIG -= release

DESTDIR = ../../lib/

RESOURCES += ../resources/resources.qrc



SOURCES += stimulator.cpp \
 teststimulator.cpp

HEADERS += stimulator.h \
 teststimulator.h

INCLUDEPATH += ../core

DISTFILES += .

