include(../../epsimulatorlibrary.pri)
TEMPLATE = lib
TARGET = recorder
SOURCES += recorder.cpp \
    patientstatusbar.cpp \
    realtimewindow.cpp \
    displaywindow.cpp \
    reviewwindow.cpp \
    logwindow.cpp \
    signalwidget.cpp
HEADERS += recorder.h \
    patientstatusbar.h \
    realtimewindow.h \
    displaywindow.h \
    reviewwindow.h \
    logwindow.h \
    recorderdefs.h \
    signalwidget.h
INCLUDEPATH += ../gui \
    ../stimulator \
    ../patient \
    ../core \
    ../hardware \
    ../study
FORMS += ui/patientstatusbar.ui
LIBS += \
    -lcore \
    -lpatient \
    -lgui \
    -lhardware \
    -lstimulator \
    -lstudy
