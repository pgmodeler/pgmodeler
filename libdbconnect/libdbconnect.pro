include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = dbconnect
OBJECTS_DIR = obj

LIBS += $$DESTDIR/$$LIBUTILS

HEADERS += src/dbconnection.h \
	   src/resultset.h

SOURCES += src/dbconnection.cpp \
	   src/resultset.cpp
