include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = utils

!macx {
 # Check if LIBDESTDIR points to another location other than DESTDIR
 # in this case the INSTALLS will be used
 !equals(LIBDESTDIR, $$DESTDIR) {
  target.path = $$LIBDESTDIR
  INSTALLS = target
 }
}

macx:DESTDIR=$$LIBDESTDIR

HEADERS += src/exception.h \
	   src/globalattributes.h \
	   src/utf8string.h \
    src/pgsqlversions.h

SOURCES += src/exception.cpp

