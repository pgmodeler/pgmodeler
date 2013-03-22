include(../pgmodeler.pro)

TEMPLATE = app
TARGET = test
DESTDIR = ../build

LIBS += $$DESTDIR/lib/$$LIBUTIL \
        $$DESTDIR/lib/$$LIBPARSERS \
        $$DESTDIR/lib/$$LIBDBCONNECT \
        $$DESTDIR/lib/$$LIBOBJRENDERER \
        $$DESTDIR/lib/$$LIBPGMODELER \
        $$DESTDIR/lib/$$LIBPGMODELERUI

#HEADERS += $$PWD/src/application.h
SOURCES += $$PWD/src/main.cpp
