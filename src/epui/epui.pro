TEMPLATE = lib

CONFIG += dll \
debug
CONFIG -= release

DESTDIR = ../../lib

FORMS += patientdialogbase.ui

SOURCES += patientdialog.cpp

HEADERS += patientdialog.h

INCLUDEPATH += ../eppatient

