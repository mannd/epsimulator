# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# these are the headers for your project
noinst_HEADERS = epsimulator.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(EXTRACTRC) `find . -name \*.ui -o -name \*.rc` > rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/epsimulator.pot

KDE_ICON = AUTO

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = epsimulator

# the application source, library search path, and link libraries
epsimulator_SOURCES = main.cpp epsimulator.cpp
epsimulator_LDFLAGS = $(KDE_RPATH) $(all_libraries)
epsimulator_LDADD = $(LIB_KDEUI)

# this is where the desktop file will go 
shelldesktopdir   = $(kde_appsdir)/Utilities
shelldesktop_DATA = epsimulator.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir   = $(kde_datadir)/epsimulator


