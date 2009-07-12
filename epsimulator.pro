#version check qt
contains(QT_VERSION, ^[34]\.[0-2]\..*) {
    message("Cannot build Qt Creator with Qt version $$QT_VERSION")
    error("Use at least Qt 4.3")
}

SUBDIRS += src/core \
    src/patient \
    src/hardware \
    src/study \
    src/gui \
    src/stimulator \
    src/recorder \
    src/navigator \
    src/app \
    src/test \

TEMPLATE = subdirs


