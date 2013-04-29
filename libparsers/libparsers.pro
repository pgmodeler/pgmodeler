include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = parsers

#Windows specific: the QMAKE_LFLAGS does not work properly with MingW so it's necessary
#to pass manually all the used libraries to the linker
windows:LIBS += $$DESTDIR/$$LIBUTILS

HEADERS += $$PWD/src/schemaparser.h \
           $$PWD/src/xmlparser.h

SOURCES += $$PWD/src/schemaparser.cpp \
           $$PWD/src/xmlparser.cpp

