TEMPLATE = lib

CONFIG += staticlib \
 debug

FORMS += ui/stimulator.ui ui/dial.ui


CONFIG -= release

DESTDIR = ../../lib/

RESOURCES += ../resources/resources.qrc



