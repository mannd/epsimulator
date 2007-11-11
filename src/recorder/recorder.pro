

SOURCES += recorder.cpp

HEADERS += recorder.h

INCLUDEPATH += ../epsim \
../eppatient \
../epui \
../stimulator \
 /usr/include/qt \
 ../navigator
LIBS += -L../../lib \
-lepsim \
-leppatient \
-lepui \
../../lib/libstimulator.a
TARGETDEPS += ../../lib/libepsim.so \
../../lib/libeppatient.so \
../../lib/libepui.so \
../../lib/libstimulator.a \
 ../navigator/../../bin/epsimulator
CONFIG -= release

CONFIG += debug \
 dll


IMAGES += hi16-app-epsimulator.png

TEMPLATE = lib

DESTDIR = ../../lib

#The following line was inserted by qt3to4
QT +=  qt3support 


