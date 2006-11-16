DEST_DIR := ../bin
OBJ_DIR := ../src
CXXFLAGS := -pipe -O2 -fmessage-length=0 -Wall -fno-strict-aliasing -Wall -W -g -D_REENTRANT  -DQT_THREAD_SUPPORT -DQT_SHARED -DQT_TABLET_SUPPORT -DQT_NO_DEBUG
INCPATH  := -I/usr/lib/qt3/mkspecs/default -I. -I/usr/include -I$(QTDIR)/include -I$../src
LIBS     := $(SUBLIBS) -L/usr/lib/ -L$(QTDIR)/lib/ -L/usr/X11R6/lib/ -lqt-mt -lXext -lX11 -lm -lpthread
OBJECTS := 	$(OBJ_DIR)/study.o \
		$(OBJ_DIR)/heart.o 

vpath %.cpp ../src
vpath %.h ../src

.PHONY: all test
all: test

$(DEST_DIR)/runner: runner.cpp
	g++ -o $@ $< $(OBJECTS) $(CXXFLAGS) $(INCPATH) $(LIBS)


runner.cpp: MyTestSuite.h
	$(MAKE) ../bin/epsimulator
	cxxtestgen.pl --gui=QtGui -o $@ $<

test: $(DEST_DIR)/runner
	$(DEST_DIR)/runner -keep