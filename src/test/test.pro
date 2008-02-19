TEMPLATE = app



#The following line was inserted by qt3to4
QT +=  qt3support 
INCLUDEPATH += ../gui \
../stimulator \
../recorder \
 ../patient \
 ../navigator \
 ../core \
 ../hardware
LIBS += ../../lib/libstimulator.a \
-lrecorder \
 -lpatient \
 -lgui \
 -lcore \
 -L../../lib \
 -lhardware \
 -lnavigator
TARGETDEPS += ../../lib/libgui.so \
../../lib/libstimulator.a \
../../lib/librecorder.so \
 ../../lib/libpatient.so \
 ../../lib/libcore.so \
 ../../lib/libhardware.so \
 ../../lib/libnavigator.so

CONFIG += qtestlib \
 debug


SOURCES += testepsimulator.cpp

HEADERS += testepsimulator.h

QMAKE_CXXFLAGS_RELEASE += -L../navigator

TARGET = ../../bin/testepsimulator

CONFIG -= release

