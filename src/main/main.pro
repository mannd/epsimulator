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
 -L../../lib \
 -lrecorder \
 -lhardware \
 ../../lib/libstimulator.a \
 -lnavigator \
 -lstudy
TARGETDEPS += ../../lib/libcore.so \
../../lib/libpatient.so \
../../lib/libgui.so \
../../lib/librecorder.so \
 ../../lib/libhardware.so \
 ../../lib/libstimulator.a \
 ../../lib/libnavigator.so \
 ../../lib/libstudy.so
SOURCES += main.cpp


RESOURCES += ../resources/resources.qrc


QT += qt3support

CONFIG -= release

CONFIG += debug

