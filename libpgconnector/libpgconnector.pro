include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = pgconnector
OBJECTS_DIR = obj

LIBS += $$PGSQL_LIB \
	$$DESTDIR/$$LIBUTILS \
	$$DESTDIR/$$LIBPGMODELER \
	$$DESTDIR/$$LIBPARSERS

HEADERS += src/resultset.h \
	   src/connection.h \
	   src/catalog.h

SOURCES += src/resultset.cpp \
	   src/connection.cpp \
	   src/catalog.cpp

# Check if LIBDESTDIR points to another location other than DESTDIR
# in this case the INSTALLS will be used
!equals(LIBDESTDIR, $$DESTDIR) {
 target.path = $$LIBDESTDIR
 INSTALLS = target
}
