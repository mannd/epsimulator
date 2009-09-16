include(../../epsimulator.pri)

TEMPLATE = app

TARGET = $$EPSIM_TEST_TARGET
DESTDIR = $$EPSIM_APP_PATH

INCLUDEPATH += \
    ../libs/gui \
    ../libs/stimulator \
    ../libs/recorder \
    ../libs/patient \
    ../libs/navigator \
    ../libs/core \
    ../libs/hardware \
    ../libs/study

LIBS += \
    -lstimulator \
    -lpatient \
    -lgui \
    -lcore \
    -lhardware \
    -lnavigator \
    -lstudy \
    -lrecorder

CONFIG += qtestlib

SOURCES += testepsimulator.cpp

HEADERS += testepsimulator.h

RESOURCES = ../resources/resources.qrc


