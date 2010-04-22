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
    editintervalsdialog.cpp \
    editcolumnformatdialog.cpp \
    editcolumnformatsdialog.cpp \
    listselector.cpp 
HEADERS += simulatorsettingsdialog.h \
    changepassworddialog.h \
    systemdialog.h \
    passworddialog.h \
    actions.h \
    editlistdialog.h \
    abstractedititemsdialog.h \
    editintervalsdialog.h \
    guiutilities.h \
    editcolumnformatdialog.h \
    editcolumnformatsdialog.h \
    listselector.h
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
