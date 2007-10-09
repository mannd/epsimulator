TEMPLATE = lib

CONFIG += dll \
 debug

#The following line was inserted by qt3to4
QT +=  qt3support 

DESTDIR = ../../lib

INCLUDEPATH += ../eppatient \
 ../epsim \
 /usr/include/Qt

SOURCES += simulatorsettingsdialog.cpp \
 patientdialog.cpp

HEADERS += simulatorsettingsdialog.h \
 patientdialog.h


CONFIG -= release

FORMS += ui/simulatorsettingsdialog.ui \
         ui/patientdialog.ui

