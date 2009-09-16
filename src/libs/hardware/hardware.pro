include(../../epsimulatorlibrary.pri)

TEMPLATE = lib

TARGET = hardware

INCLUDEPATH += \
    ../core \
    ../patient \
    ../gui

SOURCES += \
    opticaldisk.cpp \
    selectemulateddiskdialog.cpp \
    satmonitor.cpp \
    amplifier.cpp

HEADERS += \
    opticaldisk.h \
    selectemulateddiskdialog.h \
    satmonitor.h \
    amplifier.h

FORMS += \
    ui/selectemulateddiskdialog.ui \
    ui/satmonitor.ui

LIBS += \
    -lcore
