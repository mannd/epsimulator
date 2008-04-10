TEMPLATE = lib

CONFIG += dll \
          debug

DESTDIR = ../../lib/

SOURCES += options.cpp \
           versioninfo.cpp \
           user.cpp \
 fileutilities.cpp \
 passwordhandler.cpp \
 probability.cpp
HEADERS += options.h \
           versioninfo.h \
           error.h \
           user.h \
 fileutilities.h \
 passwordhandler.h \
 probability.h \
 epdefs.h


CONFIG -= release

#QT += qt3support
QT -= gui

