include(../pgmodeler.pro)

TEMPLATE = app
TARGET = tests

LIBS += $$DESTDIR/$$LIBUTILS \
        $$DESTDIR/$$LIBPARSERS \
        $$DESTDIR/$$LIBPGCONNECTOR \
        $$DESTDIR/$$LIBOBJRENDERER \
        $$DESTDIR/$$LIBPGMODELER \
        $$DESTDIR/$$LIBPGMODELERUI

SOURCES += $$PWD/src/main.cpp

# Check if BINDESTDIR points to another location other than DESTDIR
# in this case the INSTALLS will be used
!equals(BINDESTDIR, $$DESTDIR) {
 target.path = $$BINDESTDIR
 INSTALLS = target
}
