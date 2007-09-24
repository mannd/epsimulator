DEST_DIR := ../bin
OBJ_DIR := ../src/navigator
CXXFLAGS := -pipe -O2 -fmessage-length=0 -Wall -fno-strict-aliasing -Wall -W -g -D_REENTRANT  -DQT_THREAD_SUPPORT -DQT_SHARED -DQT_TABLET_SUPPORT -DQT_NO_DEBUG
INCPATH  := -I/usr/lib/qt3/mkspecs/default -I. -I/usr/include -I$(QTDIR)/include  -I../src -I../src/navigator -I../src/epsim -I../src/epui -I../src/eppatient
LIBS     := $(SUBLIBS) -L/usr/lib/ -L$(QTDIR)/lib/ -L/usr/X11R6/lib/ -L../lib -lqt-mt -lXext -lX11 -lm -lpthread -lepsim -lepui -leppatient
OBJECTS := 	$(OBJ_DIR)/filtercatalog.o \
		$(OBJ_DIR)/moc_filtercatalog.o \
		$(OBJ_DIR)/moc_filtercatalogbase.o \
		$(OBJ_DIR)/filtercatalogbase.o \
		$(OBJ_DIR)/catalogcombobox.o \
		$(OBJ_DIR)/moc_catalogcombobox.o \
		$(OBJ_DIR)/catalog.o \
		$(OBJ_DIR)/passworddialogbase.o \
		$(OBJ_DIR)/passworddialog.o \
		$(OBJ_DIR)/moc_passworddialogbase.o \
		$(OBJ_DIR)/moc_passworddialog.o \
		$(OBJ_DIR)/GeneralHashFunctions.o \
		$(OBJ_DIR)/passwordhandler.o \
		$(OBJ_DIR)/opticaldisk.o \
		$(OBJ_DIR)/user.o \
		$(OBJ_DIR)/selectemulateddiskdialog.o \
		$(OBJ_DIR)/moc_selectemulateddiskdialog.o \
		$(OBJ_DIR)/selectemulateddiskdialogbase.o \
		$(OBJ_DIR)/moc_selectemulateddiskdialogbase.o 

vpath %.cpp ../src/navigator
vpath %.h ../src/navigator

.PHONY: all test run
all: run

$(DEST_DIR)/runner: runner.cpp
	g++ -o $@ $< $(OBJECTS) $(CXXFLAGS) $(INCPATH) $(LIBS)


runner.cpp: MyTestSuite.h 
	$(MAKE) ../bin/epsimulator
	cxxtestgen.pl --gui=QtGui -o $@ $<

run: $(DEST_DIR)/runner
	$(DEST_DIR)/runner -keep
	touch MyTestSuite.h
# this should insure that next go around MyTestSuite will be rebuilt


