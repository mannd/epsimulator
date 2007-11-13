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
-leppatient \
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
SOURCES += testqstring.cpp

CONFIG += qtestlib


