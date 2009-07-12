#version check qt
contains(QT_VERSION, ^[34]\.[0-2]\..*) {
    message("Cannot build Qt Creator with Qt version $$QT_VERSION")
    error("Use at least Qt 4.3")
}

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS = src
