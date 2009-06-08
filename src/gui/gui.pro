TEMPLATE = lib

DESTDIR = ../../lib

INCLUDEPATH += ../patient \
 ../core \
 ../study \
 ../hardware

SOURCES += simulatorsettingsdialog.cpp \
 patientdialog.cpp \
 changepassworddialog.cpp \
 systemdialog.cpp \
 passworddialog.cpp \
 actions.cpp \
 abstractmainwindow.cpp

HEADERS += simulatorsettingsdialog.h \
 patientdialog.h \
 changepassworddialog.h \
 systemdialog.h \
 passworddialog.h \
 actions.h \
 abstractmainwindow.h

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

#TARGETDEPS += ../../lib/libpatient.so \
# ../../lib/libcore.so \
# ../../lib/libstudy.so

RESOURCES += ../resources/resources.qrc
