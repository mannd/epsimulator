TEMPLATE = lib

CONFIG += debug \
 dll
CONFIG -= release

INCLUDEPATH += ../core \
../patient
LIBS += -L../../lib \
-lcore \
-lpatient
TARGETDEPS += ../../lib/libcore.so \
../../lib/libpatient.so
SOURCES += study.cpp \
 studyconfiguration.cpp

HEADERS += study.h \
 studyconfiguration.h

DESTDIR = ../../lib/

