EPSIM_APP_PATH = ../../bin
win32 { 
    EPSIM_APP_TARGET = epsimulator
    EPSIM_TEST_TARGET = testepsimulator
}
else { 
    EPSIM_APP_TARGET = epsimulator.bin
    EPSIM_TEST_TARGET = testepsimulator.bin
    EPSIM_APP_WRAPPER = epsimulator.sh
    EPSIM_TEST_WRAPPER = testepsimulator.sh
}
LIBS += -L../../lib
QT += sql

