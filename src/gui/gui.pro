TEMPLATE = lib

CONFIG += dll \
 debug

#The following line was inserted by qt3to4

DESTDIR = ../../lib

INCLUDEPATH += /usr/include/Qt \
 ../patient \
 ../core

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
-lpatient \
 -lcore
TARGETDEPS += ../../lib/libpatient.so \
 ../../lib/libcore.so

RESOURCES += ../resources/resources.qrc

