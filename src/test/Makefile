DEST_DIR := ../../bin
NAVIGATOR_DIR := ../navigator
RECORDER_DIR := ../recorder
OBJ_DIR := $(NAVIGATOR_DIR)
CXXFLAGS := -pipe -O2 -fmessage-length=0 -Wall -fno-strict-aliasing -Wall -W -g -D_REENTRANT  -DQT_THREAD_SUPPORT -DQT_SHARED -DQT_TABLET_SUPPORT -DQT_NO_DEBUG
INCPATH  := -I/usr/lib/qt3/mkspecs/default -I. -I/usr/include -I$(QTDIR)/include  -I../navigator -I../epsim -I../epui -I../eppatient
LIBS     := $(SUBLIBS) -L/usr/lib/ -L$(QTDIR)/lib/ -L/usr/X11R6/lib/ -L../../lib -lqt-mt -lXext -lX11 -lm -lpthread -lepsim -lepui -leppatient
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
TARGET = $(DEST_DIR)/runner
DEL_FILE = rm -f
SYMLINK  = ln -sf
DEL_DIR  = rmdir
MOVE     = mv -f
CHK_DIR_EXISTS= test -d
MKDIR    = mkdir -p

vpath %.cpp ../navigator
vpath %.h ../navigator

.PHONY: all test run clean distclean

all:  $(TARGET) 

$(DEST_DIR)/runner: runner.cpp 
	g++ -o $@ $< $(OBJECTS) $(CXXFLAGS) $(INCPATH) $(LIBS)

runner.cpp: MyTestSuite.h 
	cxxtestgen.pl --gui=QtGui -o $@ $<

clean:
	-$(DEL_FILE) runner.cpp

distclean: clean
	-$(DEL_FILE) $(TARGET)



