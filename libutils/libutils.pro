include(../pgmodeler.pri)

TEMPLATE = lib
TARGET = utils
windows: DESTDIR = $$PWD

HEADERS += src/exception.h \
           src/globalattributes.h \
           src/utf8string.h \
           src/pgsqlversions.h

SOURCES += src/exception.cpp

# Deployment settings
target.path = $$PRIVATELIBDIR
INSTALLS = target
