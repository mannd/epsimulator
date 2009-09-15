defineReplace(cleanPath) {
    win32:1 ~= s|\\\\|/|g
    contains(1, ^/.*):pfx = /
    else:pfx =
    segs = $$split(1, /)
    out =
    for(seg, segs) {
        equals(seg, ..):out = $$member(out, 0, -2)
        else:!equals(seg, .):out += $$seg
    }
    return($$join(out, /, $$pfx))
}

defineReplace(targetPath) {
    win32:1 ~= s|/|\|g
    return($$1)
}

EPSIM_SOURCE_TREE = $$PWD
isEmpty(EPSIM_BUILD_TREE) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    EPSIM_BUILD_TREE = $$cleanPath($$OUT_PWD)
    EPSIM_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
}
EPSIM_APP_PATH = $$EPSIM_BUILD_TREE/bin

win32 {
     EPSIM_APP_TARGET = epsimulator
}
else {
     EPSIM_APP_TARGET = epsimulator.bin
     EPSIM_APP_WRAPPER = epsimulator
}

EPSIM_LIBRARY_PATH = $$EPSIM_BUILD_PATH/lib

LIBS += -L$$EPSIM_LIBRARY_PATH

RESOURCES += $$EPSIM_SOURCE_TREE/src/resources/resources.qrc
