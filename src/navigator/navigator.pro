
FORMS += ui/filtercatalogdialog.ui \
         ui/disklabeldialog.ui \
         ui/selectstudyconfigdialog.ui \
         ui/movecopystudydialog.ui
TRANSLATIONS += epsimulator_de.ts \
                epsimulator_fr.ts 

#IDLS += ui/studymovecopywizard.ui 
HEADERS += navigator.h \
           catalogcombobox.h \
           catalog.h \
           tablelistview.h \
           disklabeldialog.h \
           buttonframe.h \
           statusbar.h \
           selectstudyconfigdialog.h \
 filtercatalogdialog.h \
 movecopystudydialog.h
SOURCES += navigator.cpp \
           catalogcombobox.cpp \
           catalog.cpp \
           tablelistview.cpp \
           disklabeldialog.cpp \
           buttonframe.cpp \
           statusbar.cpp \
           selectstudyconfigdialog.cpp \
 filtercatalogdialog.cpp \
 movecopystudydialog.cpp
TEMPLATE = lib
CONFIG += debug \
warn_on \
thread \
qt \
 dll
CONFIG -= release


INCLUDEPATH += ../gui \
 ../recorder \
 ../patient \
 ../core \
 ../hardware

#The following line was inserted by qt3to4
QT +=  qt3support 

RESOURCES += ../resources/resources.qrc

DESTDIR = ../../lib/

LIBS += -L../../lib \
-lcore \
-lpatient \
-lgui \
-lhardware \
 -lrecorder
TARGETDEPS += ../../lib/libcore.so \
../../lib/libpatient.so \
../../lib/libgui.so \
../../lib/libhardware.so \
 ../../lib/librecorder.so
