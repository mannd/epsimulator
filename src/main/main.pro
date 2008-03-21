TEMPLATE = app

CONFIG -= release

CONFIG += debug

TARGET = ../../bin/epsimulator

INCLUDEPATH += ../core \
../navigator \
 ../recorder \
 ../patient \
 ../gui \
 ../hardware \
 ../stimulator
LIBS += -lpatient \
-lgui \
-lcore \
 -L../../lib \
 -lrecorder \
 -lhardware \
 ../../lib/libstimulator.a \
 -lnavigator
TARGETDEPS += ../../lib/libcore.so \
../../lib/libpatient.so \
../../lib/libgui.so \
../../lib/librecorder.so \
 ../../lib/libhardware.so \
 ../../lib/libstimulator.a \
 ../../lib/libnavigator.so
SOURCES += main.cpp


RESOURCES += ../resources/resources.qrc


QT += qt3support

