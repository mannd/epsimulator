
FORMS += ui/filtercatalogdialog.ui \
         ui/disklabeldialog.ui \
         ui/movecopystudywizard.ui  \
         ui/selectemulateddiskdialog.ui \
         ui/selectstudyconfigdialog.ui \
         ui/movecopystudydialog.ui
TRANSLATIONS += epsimulator_de.ts \
                epsimulator_fr.ts 

#IDLS += ui/studymovecopywizard.ui 
HEADERS += navigator.h \
           catalogcombobox.h \
           catalog.h \
           tablelistview.h \
           opticaldisk.h \
           disklabeldialog.h \
           buttonframe.h \
           statusbar.h \
           selectemulateddiskdialog.h \
 selectstudyconfigdialog.h \
 filtercatalogdialog.h \
 movecopystudydialog.h \
 movecopystudywizard.h
SOURCES += navigator.cpp \
           catalogcombobox.cpp \
           catalog.cpp \
           tablelistview.cpp \
           opticaldisk.cpp \
           disklabeldialog.cpp \
           buttonframe.cpp \
           statusbar.cpp \
           selectemulateddiskdialog.cpp \
 selectstudyconfigdialog.cpp \
 filtercatalogdialog.cpp \
 movecopystudydialog.cpp \
 movecopystudywizard.cpp
TEMPLATE = lib
CONFIG += debug \
warn_on \
thread \
qt \
 staticlib
CONFIG -= release

LIBS += -L../../lib \
 -lgui \
 -lrecorder \
 -lpatient \
 -lcore
TARGETDEPS += ../../lib/librecorder.so \
 ../../lib/libpatient.so \
 ../../lib/libgui.so \
 ../../lib/libcore.so

INCLUDEPATH += ../gui \
 ../recorder \
 ../patient \
 ../core

#The following line was inserted by qt3to4
QT +=  qt3support 

RESOURCES += ../resources/resources.qrc

DESTDIR = ../../lib/

