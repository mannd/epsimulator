TEMPLATE = app

QT +=  qt3support
INCLUDEPATH += ../gui \
../stimulator \
../recorder \
 ../patient \
 ../navigator \
 ../core \
 ../hardware \
 ../study

LIBS += -lstimulator \
-lpatient \
 -lgui \
 -lcore \
 -lhardware \
 -lnavigator \
 -lstudy \
 -lrecorder \
 -L../../lib

CONFIG += qtestlib

SOURCES += testepsimulator.cpp

HEADERS += testepsimulator.h

TARGET = ../../bin/testepsimulator

