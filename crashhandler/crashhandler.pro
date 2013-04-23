include(../pgmodeler.pro)

TEMPLATE = app
TARGET = crashhandler
DESTDIR = ../build

LIBS += $$DESTDIR/lib/$$LIBUTILS \
        $$DESTDIR/lib/$$LIBPARSERS \
        $$DESTDIR/lib/$$LIBDBCONNECT \
        $$DESTDIR/lib/$$LIBOBJRENDERER \
        $$DESTDIR/lib/$$LIBPGMODELER \
        $$DESTDIR/lib/$$LIBPGMODELERUI

FORMS += $$PWD/ui/crashhandler.ui

SOURCES += $$PWD/src/main.cpp \
           $$PWD/src/crashhandler.cpp

HEADERS += $$PWD/src/crashhandler.h
