include(../../epsimulatorlibrary.pri)
TEMPLATE = lib
TARGET = study
INCLUDEPATH += ../core \
    ../patient \
    ../hardware
LIBS += -lcore \
    -lpatient \
    -lhardware
SOURCES += study.cpp \
    studyconfiguration.cpp \
    studyinformationdialog.cpp
HEADERS += study.h \
    studyconfiguration.h \
    studyinformationdialog.h
FORMS += ui/studyinformationdialog.ui
