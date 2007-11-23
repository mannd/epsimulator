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
 -lrecorder
TARGETDEPS += ../../lib/libcore.so \
../../lib/libpatient.so \
../../lib/libgui.so \
../../lib/libnavigator.a \
 ../../lib/librecorder.so
SOURCES += main.cpp

QT += qt3support

