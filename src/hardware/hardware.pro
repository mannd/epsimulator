TEMPLATE = lib

CONFIG += dll

INCLUDEPATH += ../core \
../patient \
../gui
SOURCES += opticaldisk.cpp \
 selectemulateddiskdialog.cpp \
 satmonitor.cpp

HEADERS += opticaldisk.h \
 selectemulateddiskdialog.h \
 satmonitor.h

FORMS += ui/selectemulateddiskdialog.ui \
 ui/satmonitor.ui


DESTDIR = ../../lib/

