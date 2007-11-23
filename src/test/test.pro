TEMPLATE = app



#The following line was inserted by qt3to4
QT +=  qt3support 
INCLUDEPATH += ../gui \
../stimulator \
../recorder \
 ../patient \
 ../navigator \
 ../core
LIBS += -L../../lib \
../../lib/libstimulator.a \
-lrecorder \
 -lpatient \
 -lgui \
 -lcore
TARGETDEPS += ../../lib/libgui.so \
../../lib/libstimulator.a \
../../lib/librecorder.so \
 ../../lib/libpatient.so \
 ../navigator/../../bin/epsimulator \
 ../../lib/libcore.so

CONFIG += qtestlib


SOURCES += testepsimulator.cpp

HEADERS += testepsimulator.h

QMAKE_CXXFLAGS_RELEASE += -L../navigator

TARGET = ../../bin/testepsimulator

