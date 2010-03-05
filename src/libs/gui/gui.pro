include(../../epsimulatorlibrary.pri)
TEMPLATE = lib
TARGET = gui
INCLUDEPATH += ../patient \
    ../core \
    ../study \
    ../hardware
SOURCES += simulatorsettingsdialog.cpp \
    changepassworddialog.cpp \
    systemdialog.cpp \
    passworddialog.cpp \
    actions.cpp \
    abstractmainwindow.cpp \
    editlistdialog.cpp \
    abstractedititemsdialog.cpp \
    edititemsdialog.cpp \
    editintervalsdialog.cpp \
    editcolumnformatdialog.cpp \
    editcolumnformatsdialog.cpp
HEADERS += simulatorsettingsdialog.h \
    changepassworddialog.h \
    systemdialog.h \
    passworddialog.h \
    actions.h \
    abstractmainwindow.h \
    editlistdialog.h \
    abstractedititemsdialog.h \
    edititemsdialog.h \
    editintervalsdialog.h \
    guiutilities.h \
    editcolumnformatdialog.h \
    editcolumnformatsdialog.h
FORMS += ui/simulatorsettingsdialog.ui \
    ui/changepassworddialog.ui \
    ui/systemdialog.ui \
    ui/passworddialog.ui \
    ui/editlistdialog.ui \
    ui/abstractedititemsdialog.ui \
    ui/editcolumnformatdialog.ui
LIBS += -lpatient \
    -lcore \
    -lhardware
