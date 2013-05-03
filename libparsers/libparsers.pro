include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = parsers

LIBS += $$DESTDIR/$$LIBUTILS

HEADERS += $$PWD/src/schemaparser.h \
           $$PWD/src/xmlparser.h

SOURCES += $$PWD/src/schemaparser.cpp \
           $$PWD/src/xmlparser.cpp

