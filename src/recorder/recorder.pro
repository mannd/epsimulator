

SOURCES += recorder.cpp

HEADERS += recorder.h

INCLUDEPATH += ../gui \
../stimulator \
 /usr/include/qt \
 ../patient \
 ../core
LIBS += -L../../lib \
    -lstimulator \
 -lpatient \
 -lgui \
 -lcore
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


