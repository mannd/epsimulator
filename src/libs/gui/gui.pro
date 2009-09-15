TEMPLATE = lib

DESTDIR = ../../lib

INCLUDEPATH += \
    ../patient \
    ../core \
    ../study \
    ../hardware

SOURCES += \
    simulatorsettingsdialog.cpp \
    patientdialog.cpp \
    changepassworddialog.cpp \
    systemdialog.cpp \
    passworddialog.cpp \
    actions.cpp \
    abstractmainwindow.cpp

HEADERS += \
    simulatorsettingsdialog.h \
    patientdialog.h \
    changepassworddialog.h \
    systemdialog.h \
    passworddialog.h \
    actions.h \
    abstractmainwindow.h

FORMS += \
    ui/simulatorsettingsdialog.ui \
    ui/patientdialog.ui \
    ui/changepassworddialog.ui \
    ui/systemdialog.ui \
    ui/passworddialog.ui

LIBS += \
    -lpatient \
    -lcore \
    -lstudy \
    -lhardware \
    -L../../lib

RESOURCES += ../resources/resources.qrc
