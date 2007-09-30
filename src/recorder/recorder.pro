

SOURCES += recorder.cpp

HEADERS += recorder.h

INCLUDEPATH += ../epsim \
../eppatient \
../epui \
../stimulator
LIBS += -L../../lib \
-lepsim \
-leppatient \
-lepui \
../stimulator/libstimulator.a
TARGETDEPS += ../../lib/libepsim.so \
../../lib/libeppatient.so \
../../lib/libepui.so \
../stimulator/libstimulator.a
CONFIG -= release

CONFIG += debug \
 dll


IMAGES += hi16-app-epsimulator.png

TEMPLATE = lib

DESTDIR = ../../lib

