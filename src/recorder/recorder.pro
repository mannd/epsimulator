

SOURCES += recorder.cpp \
 patientstatusbar.cpp \
 realtimewindow.cpp \
 displaywindow.cpp \
 reviewwindow.cpp \
 logwindow.cpp \
 studyconfiguration.cpp \
 studyconfigurationdialog.cpp

HEADERS += recorder.h \
 patientstatusbar.h \
 realtimewindow.h \
 displaywindow.h \
 reviewwindow.h \
 logwindow.h \
 recorderdefs.h \
 studyconfiguration.h \
 studyconfigurationdialog.h

INCLUDEPATH += ../gui \
../stimulator \
 ../patient \
 ../core \
 ../hardware
CONFIG -= release

CONFIG += debug \
 dll


#IMAGES += hi16-app-epsimulator.png

TEMPLATE = lib

DESTDIR = ../../lib

#The following line was inserted by qt3to4
#QT +=  qt3support 

RESOURCES += ../resources/resources.qrc


FORMS += ui/patientstatusbar.ui \
 ui/studyconfigurationdialog.ui

LIBS += -L../../lib \
-lcore \
-lpatient \
-lgui \
-lhardware \
 ../../lib/libstimulator.a
TARGETDEPS += ../../lib/libcore.so \
../../lib/libpatient.so \
../../lib/libgui.so \
../../lib/libhardware.so \
 ../../lib/libstimulator.a
