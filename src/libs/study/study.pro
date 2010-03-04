include(../../epsimulatorlibrary.pri)
TEMPLATE = lib
TARGET = study
INCLUDEPATH += ../core \
    #../gui \
    ../patient \
    ../hardware \
    ../recorder
LIBS += -lcore \
    #-lgui \
    -lpatient \
    -lhardware \
    -lrecorder
SOURCES += study.cpp \
    studyinformationdialog.cpp
HEADERS += study.h \
    studyinformationdialog.h
FORMS += ui/studyinformationdialog.ui
