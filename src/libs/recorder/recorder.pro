include(../../epsimulatorlibrary.pri)
TEMPLATE = lib
TARGET = recorder
SOURCES += recorder.cpp \
    patientdialog.cpp \
    patientstatusbar.cpp \
    realtimewindow.cpp \
    displaywindow.cpp \
    reviewwindow.cpp \
    logwindow.cpp \
    signalwidget.cpp \
    study.cpp \
    selectstudyconfigdialog.cpp \
    studyconfigurationdialog.cpp \
    studyinformationdialog.cpp \
    studyconfiguration.cpp \
    windowsetting.cpp
HEADERS += recorder.h \
    patientdialog.h \
    patientstatusbar.h \
    realtimewindow.h \
    displaywindow.h \
    reviewwindow.h \
    logwindow.h \
    recorderdefs.h \
    signalwidget.h \
    studyconfiguration.h \
    studyconfigurationdialog.h \
    selectstudyconfigdialog.h \
    study.h \
    studyinformationdialog.h \
    windowsetting.h
INCLUDEPATH += ../gui \
    ../stimulator \
    ../patient \
    ../core \
    ../hardware \
FORMS += ui/patientstatusbar.ui \
    ui/patientdialog.ui \
    ui/studyinformationdialog.ui \
    ui/selectstudyconfigdialog.ui \
    ui/studyconfigurationdialog.ui
LIBS += -lcore \
    -lpatient \
    -lgui \
    -lhardware \
    -lstimulator

