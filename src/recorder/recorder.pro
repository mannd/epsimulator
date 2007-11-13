

SOURCES += recorder.cpp

HEADERS += recorder.h

INCLUDEPATH += ../epsim \
../epui \
../stimulator \
 /usr/include/qt \
 ../patient
LIBS += -L../../lib \
-lepsim \
-leppatient \
-lepui \
../../lib/libstimulator.a \
 -lpatient
TARGETDEPS += ../../lib/libepsim.so \
../../lib/libepui.so \
../../lib/libstimulator.a \
 ../../lib/libpatient.so
CONFIG -= release

CONFIG += debug \
 dll


IMAGES += hi16-app-epsimulator.png

TEMPLATE = lib

DESTDIR = ../../lib

#The following line was inserted by qt3to4
QT +=  qt3support 


