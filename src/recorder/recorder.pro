

SOURCES += recorder.cpp \
 patientstatusbar.cpp \
 realtimewindow.cpp

HEADERS += recorder.h \
 patientstatusbar.h \
 realtimewindow.h

INCLUDEPATH += ../gui \
../stimulator \
 ../patient \
 ../core \
 ../navigator \
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


FORMS += ui/patientstatusbar.ui

LIBS += -L../../lib \
-lcore \
-lpatient \
-lgui \
-lhardware
TARGETDEPS += ../../lib/libcore.so \
../../lib/libpatient.so \
../../lib/libgui.so \
../../lib/libhardware.so
