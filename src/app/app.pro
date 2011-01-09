include(../../epsimulator.pri)
include(../version.pri)
TEMPLATE = app
TARGET = $$EPSIM_APP_TARGET
DESTDIR = $$EPSIM_APP_PATH
INCLUDEPATH += ../libs/core \
    ../libs/navigator \
    ../libs/recorder \
    ../libs/patient \
    ../libs/gui \
    ../libs/hardware \
    ../libs/stimulator
LIBS += -lpatient \
    -lgui \
    -lcore \
    -lhardware \
    -lstimulator \
    -lnavigator \
    -lrecorder
SOURCES += main.cpp
RESOURCES = ../resources/resources.qrc

# Build database

DATABASEPATH = ../../db/en/
DATABASE = $${DATABASEPATH}epsimulator.db
DATABASESCRIPT = $${DATABASEPATH}databasescript.sh
DATABASESCHEMA = $${DATABASEPATH}databaseschema.sql

make_database.target = $$DATABASE
make_database.depends = $$DATABASESCRIPT $$DATABASESCHEMA
make_database.commands = -($$DATABASESCRIPT)

# Build empty study database
STUDYDBPATH = ../../db/
STUDYDB = $${STUDYDBPATH}emptystudy.db
STUDYDBSCRIPT = $${STUDYDBPATH}createstudydb.sh
STUDYDBSCHEMA = $${STUDYDBPATH}studydbschema.sql

make_studydb.target = $$STUDYDB
make_studydb.depends = $$STUDYDBSCRIPT $$STUDYDBSCHEMA
make_studydb.commands = -($$STUDYDBSCRIPT)

QMAKE_EXTRA_TARGETS += make_database make_studydb
PRE_TARGETDEPS += $$DATABASE $$STUDYDB
