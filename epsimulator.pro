#version check qt
contains(QT_VERSION, ^[34]\.[0-4]\..*) {
    message("Cannot build Qt Creator with Qt version $$QT_VERSION")
    error("Use at least Qt 4.5")
}

include(epsimulator.pri)

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = src
