TEMPLATE = app

CONFIG -= release

CONFIG += debug

TARGET = ../../bin/epsimulator

INCLUDEPATH += ../core \
../navigator
LIBS += -lpatient \
-lgui \
-lcore \
 ../../lib/libnavigator.a \
 -L../../lib \
 -lrecorder \
 -lhardware \
 ../../lib/libstimulator.a
TARGETDEPS += ../../lib/libcore.so \
../../lib/libpatient.so \
../../lib/libgui.so \
../../lib/libnavigator.a \
 ../../lib/librecorder.so \
 ../../lib/libhardware.so \
 ../../lib/libstimulator.a
SOURCES += main.cpp


RESOURCES += ../resources/resources.qrc


QT += qt3support

