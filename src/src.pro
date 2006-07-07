SOURCES += epsimulator.cpp \
           main.cpp
HEADERS += epsimulator.h
TEMPLATE += app
CONFIG += release \
          warn_on \
          thread \
          qt
TARGET = ../bin/epsimulator
