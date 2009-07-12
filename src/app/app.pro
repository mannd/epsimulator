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
SOURCES += main.cpp
RESOURCES += ../resources/resources.qrc
