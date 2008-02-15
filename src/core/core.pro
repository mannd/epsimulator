TEMPLATE = lib

CONFIG += dll \
          debug

DESTDIR = ../../lib/

SOURCES += options.cpp \
           settings.cpp \
           versioninfo.cpp \
           user.cpp \
 fileutilities.cpp \
 passwordhandler.cpp \
 probability.cpp
HEADERS += options.h \
           settings.h \
           versioninfo.h \
           error.h \
           user.h \
 fileutilities.h \
 passwordhandler.h \
 probability.h


CONFIG -= release

#QT += qt3support
QT -= gui

