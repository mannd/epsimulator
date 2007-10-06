TEMPLATE = lib

CONFIG += dll

DESTDIR = ../../lib/

SOURCES += getopts.cpp \
options.cpp \
settings.cpp \
versioninfo.cpp \
 epfuns.cpp \
 actions.cpp
HEADERS += getopts.h \
options.h \
settings.h \
versioninfo.h \
 epfuns.h \
 error.h \
 actions.h
#The following line was inserted by qt3to4
#QT +=  qt3support 
INCLUDEPATH += /usr/include/Qt

