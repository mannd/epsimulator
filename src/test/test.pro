TEMPLATE = app



#The following line was inserted by qt3to4
QT +=  qt3support 
INCLUDEPATH += ../epsim \
../epui \
../stimulator \
../recorder \
 ../patient \
 ../navigator
LIBS += -L../../lib \
-lepsim \
-lepui \
../../lib/libstimulator.a \
-lrecorder \
 -lpatient
TARGETDEPS += ../../lib/libepsim.so \
../../lib/libepui.so \
../../lib/libstimulator.a \
../../lib/librecorder.so \
 ../../lib/libpatient.so \
 ../navigator/../../bin/epsimulator

CONFIG += qtestlib


SOURCES += testepsimulator.cpp

HEADERS += testepsimulator.h

QMAKE_CXXFLAGS_RELEASE += -L../navigator

