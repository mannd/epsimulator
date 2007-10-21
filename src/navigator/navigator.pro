# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src/navigator
# Target is an application:  ../../bin/epsimulator

FORMS += ui/filtercatalogdialog.ui \
         ui/disklabeldialog.ui \
         ui/movecopystudywizard.ui  \
         ui/selectemulateddiskdialog.ui \
         ui/selectstudyconfigdialog.ui
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
 filtercatalogdialog.h
SOURCES += main.cpp \
           navigator.cpp \
           catalogcombobox.cpp \
           catalog.cpp \
           tablelistview.cpp \
           opticaldisk.cpp \
           disklabeldialog.cpp \
           buttonframe.cpp \
           statusbar.cpp \
           selectemulateddiskdialog.cpp \
 selectstudyconfigdialog.cpp \
 filtercatalogdialog.cpp
TEMPLATE += app
CONFIG += debug \
warn_on \
thread \
qt
TARGET = ../../bin/epsimulator
CONFIG -= release

LIBS += -lepsim \
 -L../../lib \
 -lepui \
 -leppatient \
 -lrecorder
TARGETDEPS += ../../lib/libepsim.so \
 ../../lib/libepui.so \
 ../../lib/libeppatient.so \
 ../../lib/librecorder.so

INCLUDEPATH += ../epsim \
 ../epui \
 ../eppatient \
 ../recorder \
 /usr/include/Qt

#The following line was inserted by qt3to4
QT +=  qt3support 

RESOURCES += ../resources/resources.qrc

