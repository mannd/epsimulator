include(../../epsimulatorlibrary.pri)
TEMPLATE = lib
TARGET = gui
INCLUDEPATH += ../patient \
    ../core \
    ../hardware
SOURCES += simulatorsettingsdialog.cpp \
    changepassworddialog.cpp \
    systemdialog.cpp \
    passworddialog.cpp \
    actions.cpp \
    editlistdialog.cpp \
    abstractedititemsdialog.cpp \
    editcolumnformatdialog.cpp \
    editcolumnformatsdialog.cpp \
    listselector.cpp \
    editintervalsdialog.cpp
win32:SOURCES += ../../os/guiutilities_win.cpp
unix:SOURCES += ../../os/guiutilities_unix.cpp
HEADERS += simulatorsettingsdialog.h \
    changepassworddialog.h \
    systemdialog.h \
    passworddialog.h \
    actions.h \
    editlistdialog.h \
    abstractedititemsdialog.h \
    guiutilities.h \
    editcolumnformatdialog.h \
    editcolumnformatsdialog.h \
    listselector.h \
    guiutilities.h \
    editintervalsdialog.h
FORMS += ui/simulatorsettingsdialog.ui \
    ui/changepassworddialog.ui \
    ui/systemdialog.ui \
    ui/passworddialog.ui \
    ui/editlistdialog.ui \
    ui/abstractedititemsdialog.ui \
    ui/editcolumnformatdialog.ui  \
    ui/edititemsdialog.ui
LIBS += -lpatient \
    -lcore \
    -lhardware
