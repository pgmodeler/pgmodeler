include(../pgmodeler.pro)

TEMPLATE = app
TARGET = crashhandler
DESTDIR = ../build

LIBS += $$DESTDIR/lib/$$LIBUTIL \
        $$DESTDIR/lib/$$LIBPARSERS \
        $$DESTDIR/lib/$$LIBCONEXBD \
        $$DESTDIR/lib/$$LIBOBJRENDERER \
        $$DESTDIR/lib/$$LIBPGMODELER \
        $$DESTDIR/lib/$$LIBPGMODELERUI

FORMS += ui/crashhandler.ui

SOURCES += src/main.cpp \
           src/crashhandler.cpp

HEADERS += src/crashhandler.h
