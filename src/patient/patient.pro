TEMPLATE = lib

CONFIG += dll \
debug
CONFIG -= release

DESTDIR = ../../lib/

SOURCES += heart.cpp \
study.cpp
HEADERS += heart.h \
study.h
INCLUDEPATH += ../epsim \
 /usr/include/Qt

#The following line was inserted by qt3to4
QT +=  qt3support 
LIBS += -L../../lib \
-lepsim
TARGETDEPS += ../../lib/libepsim.so

