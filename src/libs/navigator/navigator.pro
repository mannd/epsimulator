include(../../epsimulatorlibrary.pri)

TEMPLATE = lib

TARGET = navigator

FORMS += \
    ui/filtercatalogdialog.ui \
    ui/disklabeldialog.ui \
    ui/selectstudyconfigdialog.ui \
    ui/movecopystudydialog.ui

TRANSLATIONS += \
    epsimulator_de.ts \
    epsimulator_fr.ts

HEADERS += \
    navigator.h \
    catalogcombobox.h \
    catalog.h \
    tablelistview.h \
    disklabeldialog.h \
    buttonframe.h \
    statusbar.h \
    selectstudyconfigdialog.h \
    filtercatalogdialog.h \
    movecopystudydialog.h

SOURCES += \
    navigator.cpp \
    catalogcombobox.cpp \
    catalog.cpp \
    tablelistview.cpp \
    disklabeldialog.cpp \
    buttonframe.cpp \
    statusbar.cpp \
    selectstudyconfigdialog.cpp \
    filtercatalogdialog.cpp \
    movecopystudydialog.cpp

INCLUDEPATH += \
    ../gui \
    ../recorder \
    ../patient \
    ../core \
    ../hardware \
    ../study

LIBS += \
    -lcore \
    -lpatient \
    -lgui \
    -lhardware \
    -lstudy \
    -lrecorder
