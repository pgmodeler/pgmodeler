include(../pgmodeler.pro)

TEMPLATE = app
TARGET = pgmodeler-rev

LIBS += $$LIBDESTDIR/$$LIBUTILS \
        $$LIBDESTDIR/$$LIBPARSERS \
        $$LIBDESTDIR/$$LIBPGCONNECTOR \
        $$LIBDESTDIR/$$LIBOBJRENDERER \
        $$LIBDESTDIR/$$LIBPGMODELER \
        $$LIBDESTDIR/$$LIBPGMODELERUI

SOURCES += $$PWD/src/main.cpp
