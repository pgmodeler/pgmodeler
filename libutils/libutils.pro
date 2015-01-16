include(../pgmodeler.pri)

TEMPLATE = lib
TARGET = utils

HEADERS += src/exception.h \
	   src/globalattributes.h \
	   src/utf8string.h \
           src/pgsqlversions.h

SOURCES += src/exception.cpp

# Installation
target.path = $$PRIVATELIBDIR
INSTALLS = target
