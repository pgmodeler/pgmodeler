include(../pgmodeler.pro)

TEMPLATE = app
TARGET = tests

!macx {
 # Check if BINDESTDIR points to another location other than DESTDIR
 # in this case the INSTALLS will be used
 !equals(BINDESTDIR, $$DESTDIR) {
  target.path = $$BINDESTDIR
  INSTALLS = target
 }

 LIBS += $$DESTDIR/$$LIBUTILS \
         $$DESTDIR/$$LIBPARSERS \
         $$DESTDIR/$$LIBPGCONNECTOR \
         $$DESTDIR/$$LIBOBJRENDERER \
         $$DESTDIR/$$LIBPGMODELER \
         $$DESTDIR/$$LIBPGMODELERUI
}

macx {
 DESTDIR=$$BINDESTDIR
 LIBS += $$LIBDESTDIR/$$LIBUTILS \
         $$LIBDESTDIR/$$LIBPARSERS \
         $$LIBDESTDIR/$$LIBPGCONNECTOR \
         $$LIBDESTDIR/$$LIBOBJRENDERER \
         $$LIBDESTDIR/$$LIBPGMODELER \
         $$LIBDESTDIR/$$LIBPGMODELERUI
}

SOURCES += $$PWD/src/main.cpp
