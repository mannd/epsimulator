include(../../epsimulator.pri)

TEMPLATE = app

TARGET = $$EPSIM_APP_TARGET
DESTDIR = $$EPSIM_APP_PATH

INCLUDEPATH += \
    ../core \
    ../navigator \
    ../recorder \
    ../patient \
    ../gui \
    ../hardware \
    ../stimulator \
    ../study

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

