# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: ./src
# Target is an application:  ../bin/epsimulator

FORMS += patientinformationdialog.ui 
TRANSLATIONS += epsimulator_de.ts 
IMAGES += images/hi16-app-epsimulator.png \
          images/hi32-app-epsimulator.png 
HEADERS += epsimulator.h \
           patientinformationdialog.ui.h \
           navigator.h 
SOURCES += epsimulator.cpp \
           main.cpp \
           navigator.cpp 
TEMPLATE += app
CONFIG += debug \
warn_on \
thread \
qt
TARGET = ../bin/epsimulator
