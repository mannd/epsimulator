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
#    selectstudyconfigdialog.cpp \
#    studyconfigurationdialog.cpp \
    editlistdialog.cpp \
    abstractedititemsdialog.cpp \
    edititemsdialog.cpp \
    editintervalsdialog.cpp \
    editcolumnformatdialog.cpp \
    editcolumnformatsdialog.cpp
HEADERS += simulatorsettingsdialog.h \
    patientdialog.h \
    changepassworddialog.h \
    systemdialog.h \
    passworddialog.h \
    actions.h \
    abstractmainwindow.h \
#    selectstudyconfigdialog.h \
#    studyconfigurationdialog.h \
    editlistdialog.h \
    abstractedititemsdialog.h \
    edititemsdialog.h \
    editintervalsdialog.h \
    guiutilities.h \
    editcolumnformatdialog.h \
    editcolumnformatsdialog.h
FORMS += ui/simulatorsettingsdialog.ui \
    ui/patientdialog.ui \
    ui/changepassworddialog.ui \
    ui/systemdialog.ui \
    ui/passworddialog.ui \
#    ui/selectstudyconfigdialog.ui \
#    ui/studyconfigurationdialog.ui \
    ui/editlistdialog.ui \
    ui/abstractedititemsdialog.ui \
    ui/editcolumnformatdialog.ui
LIBS += -lpatient \
    -lcore \
    -lstudy \
    -lhardware
