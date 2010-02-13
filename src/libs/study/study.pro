include(../../epsimulatorlibrary.pri)
TEMPLATE = lib
TARGET = study
INCLUDEPATH += ../core \
    ../gui \
    ../patient \
    ../hardware
LIBS += -lcore \
    -lgui \
    -lpatient \
    -lhardware
SOURCES += study.cpp \
    studyconfiguration.cpp \
    studyinformationdialog.cpp
HEADERS += study.h \
    studyconfiguration.h \
    studyinformationdialog.h
FORMS += ui/studyinformationdialog.ui
