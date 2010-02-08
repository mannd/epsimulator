include(../../epsimulatorlibrary.pri)
TEMPLATE = lib
TARGET = gui
INCLUDEPATH += ../patient \
    ../core \
    ../study \
    ../hardware
SOURCES += simulatorsettingsdialog.cpp \
    patientdialog.cpp \
    changepassworddialog.cpp \
    systemdialog.cpp \
    passworddialog.cpp \
    actions.cpp \
    abstractmainwindow.cpp \
    selectstudyconfigdialog.cpp \
    studyconfigurationdialog.cpp \
    editlistdialog.cpp \
    abstractedititemsdialog.cpp \
    edititemsdialog.cpp
HEADERS += simulatorsettingsdialog.h \
    patientdialog.h \
    changepassworddialog.h \
    systemdialog.h \
    passworddialog.h \
    actions.h \
    abstractmainwindow.h \
    selectstudyconfigdialog.h \
    studyconfigurationdialog.h \
    editlistdialog.h \
    abstractedititemsdialog.h \
    edititemsdialog.h
FORMS += ui/simulatorsettingsdialog.ui \
    ui/patientdialog.ui \
    ui/changepassworddialog.ui \
    ui/systemdialog.ui \
    ui/passworddialog.ui \
    ui/selectstudyconfigdialog.ui \
    ui/studyconfigurationdialog.ui \
    ui/editlistdialog.ui \
    ui/abstractedititemsdialog.ui
LIBS += -lpatient \
    -lcore \
    -lstudy \
    -lhardware
