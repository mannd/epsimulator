TEMPLATE = lib

CONFIG += dll

DESTDIR = ../../lib/

SOURCES += getopts.cpp \
options.cpp \
settings.cpp \
versioninfo.cpp \
 epfuns.cpp \
 actions.cpp \
 utilities.cpp \
 user.cpp
HEADERS += getopts.h \
options.h \
settings.h \
versioninfo.h \
 epfuns.h \
 error.h \
 actions.h \
 utilities.h \
 user.h
#The following line was inserted by qt3to4
#QT +=  qt3support 
INCLUDEPATH += /usr/include/Qt

