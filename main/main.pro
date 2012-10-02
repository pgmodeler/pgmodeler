include(../pgmodeler.pro)

TEMPLATE = app
TARGET = pgmodeler
DESTDIR = ../build

contains(BUILDCONF, static) {
 include(../libutil/libutil.pro)
 include(../libparsers/libparsers.pro)
 include(../libconexbd/libconexbd.pro)
 include(../libpgmodeler/libpgmodeler.pro)
 include(../libobjrenderer/libobjrenderer.pro)
 include(../libpgmodeler_ui/libpgmodeler_ui.pro)
 INCLUDEPATH = $$unique(INCLUDEPATH)
}

contains(BUILDCONF, shared) {
 LIBS += $$DESTDIR/lib/$$LIBUTIL \
         $$DESTDIR/lib/$$LIBPARSERS \
         $$DESTDIR/lib/$$LIBCONEXBD \
         $$DESTDIR/lib/$$LIBOBJRENDERER \
         $$DESTDIR/lib/$$LIBPGMODELER \
         $$DESTDIR/lib/$$LIBPGMODELERUI
}

HEADERS += $$PWD/src/aplicacao.h
SOURCES += $$PWD/src/main.cpp

message("** Building configuration: [$$BUILDCONF] **")
message("** Type 'make' to start build. **")
