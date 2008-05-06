TEMPLATE = lib

CONFIG += dll \
 debug

#The following line was inserted by qt3to4

DESTDIR = ../../lib

INCLUDEPATH += ../patient \
 ../core \
 ../study

SOURCES += simulatorsettingsdialog.cpp \
 patientdialog.cpp \
 changepassworddialog.cpp \
 systemdialog.cpp \
 passworddialog.cpp \
 actions.cpp

HEADERS += simulatorsettingsdialog.h \
 patientdialog.h \
 changepassworddialog.h \
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

LIBS += -lpatient \
 -lcore \
 -L../../lib \
 -lstudy
TARGETDEPS += ../../lib/libpatient.so \
 ../../lib/libcore.so \
 ../../lib/libstudy.so

RESOURCES += ../resources/resources.qrc




