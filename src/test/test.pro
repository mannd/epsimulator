include(../../epsimulator.pri)
TEMPLATE = app
TARGET = $$EPSIM_TEST_TARGET
DESTDIR = $$EPSIM_APP_PATH
INCLUDEPATH += ../libs/gui \
    ../libs/stimulator \
    ../libs/recorder \
    ../libs/patient \
    ../libs/navigator \
    ../libs/core \
    ../libs/hardware
LIBS += -lstimulator \
    -lpatient \
    -lgui \
    -lcore \
    -lhardware \
    -lnavigator \
    -lrecorder
CONFIG += qtestlib
SOURCES += testepsimulator.cpp
HEADERS += testepsimulator.h \
    mockdatastream.h
RESOURCES = ../resources/resources.qrc
