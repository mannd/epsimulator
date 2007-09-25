TEMPLATE = lib

CONFIG += dll \
debug
CONFIG -= release

DESTDIR = ../../lib

FORMS += patientdialogbase.ui \
 simulatorsettingsdialogbase.ui

SOURCES += patientdialog.cpp \
 simulatorsettingsdialog.cpp

HEADERS += patientdialog.h \
 simulatorsettingsdialog.h

INCLUDEPATH += ../eppatient \
 ../epsim

