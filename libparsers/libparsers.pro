include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = parsers

windows:LIBS += $$DESTDIR/$$LIBUTILS

HEADERS += $$PWD/src/schemaparser.h \
           $$PWD/src/xmlparser.h

SOURCES += $$PWD/src/schemaparser.cpp \
           $$PWD/src/xmlparser.cpp

