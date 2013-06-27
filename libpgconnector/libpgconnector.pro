include(../pgmodeler.pro)

DESTDIR=$$LIBDESTDIR
TEMPLATE = lib
TARGET = pgconnector
OBJECTS_DIR = obj

LIBS += $$DESTDIR/$$LIBUTILS

HEADERS += src/resultset.h \
	   src/connection.h \
	   src/catalog.h

SOURCES += src/resultset.cpp \
	   src/connection.cpp \
	   src/catalog.cpp
