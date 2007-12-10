

SOURCES += recorder.cpp

HEADERS += recorder.h

INCLUDEPATH += ../gui \
../stimulator \
 ../patient \
 ../core \
 ../navigator
LIBS += -lpatient \
 -lgui \
 -lcore \
 -L../../lib \
 ../../lib/libstimulator.a \
 ../../lib/libnavigator.a
TARGETDEPS += ../../lib/libgui.so \
../../lib/libstimulator.a \
 ../../lib/libpatient.so \
 ../../lib/libcore.so \
 ../../lib/libnavigator.a
CONFIG -= release

CONFIG += debug \
 dll


#IMAGES += hi16-app-epsimulator.png

TEMPLATE = lib

DESTDIR = ../../lib

#The following line was inserted by qt3to4
#QT +=  qt3support 

RESOURCES += ../resources/resources.qrc


