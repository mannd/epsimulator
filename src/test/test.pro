TEMPLATE = app

QT +=  qt3support 
INCLUDEPATH += ../gui \
../stimulator \
../recorder \
 ../patient \
 ../navigator \
 ../core \
 ../hardware \
 ../study
LIBS += ../../lib/libstimulator.a \
-lpatient \
 -lgui \
 -lcore \
 -lhardware \
 -lnavigator \
 -lstudy \
 -lrecorder \
 -L../../lib

TARGETDEPS += ../../lib/libgui.so \
../../lib/libstimulator.a \
../../lib/libpatient.so \
 ../../lib/libcore.so \
 ../../lib/libhardware.so \
 ../../lib/libnavigator.so \
 ../../lib/libstudy.so \
 ../../lib/librecorder.so

CONFIG += qtestlib

SOURCES += testepsimulator.cpp

HEADERS += testepsimulator.h

TARGET = ../../bin/testepsimulator

