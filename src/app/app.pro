include(../../epsimulator.pri)

TEMPLATE = app

TARGET = $$EPSIM_APP_TARGET
DESTDIR = $$EPSIM_APP_PATH

INCLUDEPATH += \
    ../libs/core \
    ../libs/navigator \
    ../libs/recorder \
    ../libs/patient \
    ../libs/gui \
    ../libs/hardware \
    ../libs/stimulator \
    ../libs/study

LIBS += \
    -lpatient \
    -lgui \
    -lcore \
    -lhardware \
    -lstimulator \
    -lnavigator \
    -lstudy \
    -lrecorder

SOURCES += main.cpp

