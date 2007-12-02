TEMPLATE = lib

CONFIG += dll \
          debug

DESTDIR = ../../lib/

SOURCES += options.cpp \
           settings.cpp \
           versioninfo.cpp \
           user.cpp \
 fileutilities.cpp
HEADERS += options.h \
           settings.h \
           versioninfo.h \
           error.h \
           user.h \
 fileutilities.h


CONFIG -= release

#QT += qt3support
QT -= gui

