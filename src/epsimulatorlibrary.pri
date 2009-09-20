include(../epsimulator.pri)

win32 {
      DLLDESTDIR = ../../../bin
}

DESTDIR = ../../../lib
LIBS -= -L../../lib
LIBS += -L../../../lib

RESOURCES = ../../resources/resources.qrc
