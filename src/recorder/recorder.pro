

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
 ../navigator
LIBS += -lpatient \
 -lgui \
 -lcore \
 -L../../lib \
 ../../lib/libstimulator.a
TARGETDEPS += ../../lib/libgui.so \
../../lib/libstimulator.a \
 ../../lib/libpatient.so \
 ../../lib/libcore.so
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

