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
CATALOGDBPATH = ../../db/
CATALOGDB = $${CATALOGDBPATH}emptycatalog.db
CATALOGDBSCRIPT = $${CATALOGDBPATH}createcatalogdb.sh
CATALOGDBSCHEMA = $${CATALOGDBPATH}catalogdbschema.sql

make_catalogdb.target = $$CATALOGDB
make_catalogdb.depends = $$CATALOGDBSCRIPT $$CATALOGDBSCHEMA
make_catalogdb.commands = -($$CATALOGDBSCRIPT)

QMAKE_EXTRA_TARGETS += make_database make_catalogdb
PRE_TARGETDEPS += $$DATABASE $$CATALOGDB
