TEMPLATE = lib

CONFIG += dll \
 debug

#The following line was inserted by qt3to4
QT +=  qt3support 

DESTDIR = ../../lib

INCLUDEPATH += ../eppatient \
 ../epsim

SOURCES += simulatorsettingsdialog.cpp

HEADERS += simulatorsettingsdialog.h


CONFIG -= release

FORMS += simulatorsettingsdialog.ui

