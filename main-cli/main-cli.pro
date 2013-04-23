include(../pgmodeler.pro)

TEMPLATE = app
TARGET = pgmodeler-cli
DESTDIR = ../build

INCLUDEPATH += ../main/src

LIBS += $$DESTDIR/lib/$$LIBUTIL \
        $$DESTDIR/lib/$$LIBPARSERS \
        $$DESTDIR/lib/$$LIBDBCONNECT \
        $$DESTDIR/lib/$$LIBOBJRENDERER \
        $$DESTDIR/lib/$$LIBPGMODELER \
        $$DESTDIR/lib/$$LIBPGMODELERUI

SOURCES += $$PWD/src/main.cpp \
	   $$PWD/src/pgmodelercli.cpp

HEADERS += $$PWD/src/pgmodelercli.h
