include(../../epsimulatorlibrary.pri)
TEMPLATE = lib
TARGET = patient
SOURCES += heart.cpp \
    saturation.cpp \
    patient.cpp \
    bloodpressure.cpp
HEADERS += heart.h \
    saturation.h \
    patient.h \
    bloodpressure.h
INCLUDEPATH += ../core
LIBS += -lcore
QT -= gui
