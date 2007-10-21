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
 patientdialog.cpp \
 changepassworddialog.cpp \
 passwordhandler.cpp \
 GeneralHashFunctions.cpp \
 systemdialog.cpp

HEADERS += simulatorsettingsdialog.h \
 patientdialog.h \
 changepassworddialog.h \
 passwordhandler.h \
 GeneralHashFunctions.h \
 systemdialog.h


CONFIG -= release

FORMS += ui/simulatorsettingsdialog.ui \
         ui/patientdialog.ui \
         ui/changepassworddialog.ui \
         ui/systemdialog.ui

DISTFILES += .

LIBS += -L../../lib \
-lepsim
TARGETDEPS += ../../lib/libepsim.so


