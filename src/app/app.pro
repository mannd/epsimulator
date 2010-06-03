include(../../epsimulator.pri)
TEMPLATE = app
TARGET = $$EPSIM_APP_TARGET
DESTDIR = $$EPSIM_APP_PATH
INCLUDEPATH += ../libs/core \
    ../libs/navigator \
    ../libs/recorder \
    ../libs/patient \
    ../libs/gui \
    ../libs/hardware \
    ../libs/stimulator
LIBS += -lpatient \
    -lgui \
    -lcore \
    -lhardware \
    -lstimulator \
    -lnavigator \
    -lrecorder
SOURCES += main.cpp
RESOURCES = ../resources/resources.qrc
QMAKE_CXXFLAGS += -DAPP_DATE=\\\"`date +'\"%a_%b_%d,_%Y\"'`\\\"
