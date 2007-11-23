TEMPLATE = lib

CONFIG += dll \
          debug

DESTDIR = ../../lib/

SOURCES += options.cpp \
           settings.cpp \
           versioninfo.cpp \
           epfuns.cpp \
           utilities.cpp \
           user.cpp
HEADERS += options.h \
           settings.h \
           versioninfo.h \
           epfuns.h \
           error.h \
           utilities.h \
           user.h

INCLUDEPATH += /usr/include/Qt

CONFIG -= release

#QT += qt3support
QT -= gui

