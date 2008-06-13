TEMPLATE = app



TARGET = ../../bin/epsimulator

INCLUDEPATH += ../core \
../navigator \
 ../recorder \
 ../patient \
 ../gui \
 ../hardware \
 ../stimulator \
 ../study
LIBS += -lpatient \
-lgui \
-lcore \
 -lhardware \
 ../../lib/libstimulator.a \
 -lnavigator \
 -lstudy \
 -lrecorder \
 -L../../lib
TARGETDEPS += ../../lib/libcore.so \
../../lib/libpatient.so \
../../lib/libgui.so \
../../lib/libhardware.so \
 ../../lib/libstimulator.a \
 ../../lib/libnavigator.so \
 ../../lib/libstudy.so \
 ../../lib/librecorder.so
SOURCES += main.cpp


RESOURCES += ../resources/resources.qrc


QT += qt3support

CONFIG -= release

CONFIG += debug

