TEMPLATE = lib

CONFIG += debug \
 dll
CONFIG -= release

INCLUDEPATH += ../core \
../patient \
 ../hardware
LIBS += -L../../lib \
-lcore \
-lpatient \
 -lhardware
TARGETDEPS += ../../lib/libcore.so \
../../lib/libpatient.so \
 ../../lib/libhardware.so
SOURCES += study.cpp \
 studyconfiguration.cpp

HEADERS += study.h \
 studyconfiguration.h

DESTDIR = ../../lib/

