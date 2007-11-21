TEMPLATE = lib

CONFIG += dll \
 debug

#The following line was inserted by qt3to4
QT +=  qt3support 

DESTDIR = ../../lib

INCLUDEPATH += ../epsim \
 /usr/include/Qt \
 ../patient

SOURCES += simulatorsettingsdialog.cpp \
 patientdialog.cpp \
 changepassworddialog.cpp \
 passwordhandler.cpp \
 systemdialog.cpp \
 passworddialog.cpp \
 actions.cpp

HEADERS += simulatorsettingsdialog.h \
 patientdialog.h \
 changepassworddialog.h \
 passwordhandler.h \
 systemdialog.h \
 passworddialog.h \
 actions.h


CONFIG -= release

FORMS += ui/simulatorsettingsdialog.ui \
         ui/patientdialog.ui \
         ui/changepassworddialog.ui \
         ui/systemdialog.ui \
         ui/passworddialog.ui

DISTFILES += .

LIBS += -L../../lib \
-lepsim \
 -lpatient
TARGETDEPS += ../../lib/libepsim.so \
 ../../lib/libpatient.so


