include(../../epsimulatorlibrary.pri)
TEMPLATE = lib
TARGET = navigator
FORMS += ui/filtercatalogdialog.ui \
    ui/disklabeldialog.ui \
    ui/editstudyconfigsdialog.ui \
    ui/movecopystudydialog.ui
TRANSLATIONS += epsimulator_de.ts \
    epsimulator_fr.ts
HEADERS += navigator.h \
    catalogcombobox.h \
    catalog.h \
    tablelistview.h \
    disklabeldialog.h \
    buttonframe.h \
    statusbar.h \
    filtercatalogdialog.h \
    movecopystudydialog.h \
    editstudyconfigsdialog.h \
    studytable.h
SOURCES += navigator.cpp \
    catalogcombobox.cpp \
    catalog.cpp \
    tablelistview.cpp \
    disklabeldialog.cpp \
    buttonframe.cpp \
    statusbar.cpp \
    filtercatalogdialog.cpp \
    movecopystudydialog.cpp \
    editstudyconfigsdialog.cpp \
    studytable.cpp
INCLUDEPATH += ../gui \
    ../recorder \
    ../patient \
    ../core \
    ../hardware
LIBS += -lcore \
    -lpatient \
    -lgui \
    -lhardware \
    -lrecorder
