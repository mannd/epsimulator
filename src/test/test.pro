TEMPLATE = app



#The following line was inserted by qt3to4
QT +=  qt3support 
INCLUDEPATH += ../epsim \
../eppatient \
../epui \
../stimulator \
../recorder
LIBS += -L../../lib \
-lepsim \
-leppatient \
-lepui \
../../lib/libstimulator.a \
-lrecorder
TARGETDEPS += ../../lib/libepsim.so \
../../lib/libeppatient.so \
../../lib/libepui.so \
../../lib/libstimulator.a \
../../lib/librecorder.so
SOURCES += testqstring.cpp

CONFIG += qtestlib


