include(../pgmodeler.pro)

DESTDIR=$$LIBDESTDIR
TEMPLATE = lib
TARGET = utils

HEADERS += src/exception.h \
	   src/globalattributes.h \
	   src/utf8string.h

SOURCES += src/exception.cpp
