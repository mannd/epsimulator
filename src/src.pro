# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src
# Target is an application:  ../bin/epsimulator

FORMS += patientdialogbase.ui \
         systemdialogbase.ui \
         studyconfigdialogbase.ui \
         filtercatalogbase.ui \
         disklabeldialogbase.ui 
TRANSLATIONS += epsimulator_de.ts \
                epsimulator_fr.ts 
IMAGES += images/hi16-app-epsimulator.png \
          images/hi32-app-epsimulator.png \
          images/hi32-newstudy.png \
          images/hi32-continuestudy.png \
          images/hi32-preregister.png \
          images/hi32-reports.png \
          images/hi32-reviewstudy.png \
          images/hi32-filterstudies.png \
          images/hi32-refreshcatalog.png \
          images/hi32-removefilter.png \
          images/hi32-exportstudy.png \
          images/hi64-newstudy.png \
          images/hi64-continuestudy.png \
          images/hi64-preregister.png \
          images/hi64-reports.png \
          images/hi64-reviewstudy.png 
HEADERS += epsimulator.h \
           navigator.h \
           patientdialog.h \
           study.h \
           heart.h \
           systemdialog.h \
           studyconfigdialog.h \
           options.h \
           getopts.h \
           epsimdefs.h \
           filtercatalog.h \
           catalogcombobox.h \
           catalog.h \
           tablelistview.h \
           opticaldisk.h \
           settings.h \
           disklabeldialog.h \
           buttonframe.h \
           statusbar.h 
SOURCES += epsimulator.cpp \
           main.cpp \
           navigator.cpp \
           patientdialog.cpp \
           study.cpp \
           heart.cpp \
           systemdialog.cpp \
           studyconfigdialog.cpp \
           options.cpp \
           getopts.cpp \
           filtercatalog.cpp \
           catalogcombobox.cpp \
           catalog.cpp \
           tablelistview.cpp \
           opticaldisk.cpp \
           settings.cpp \
           disklabeldialog.cpp \
           buttonframe.cpp \
           statusbar.cpp 
TEMPLATE += app
CONFIG += debug \
warn_on \
thread \
qt
TARGET = ../bin/epsimulator
