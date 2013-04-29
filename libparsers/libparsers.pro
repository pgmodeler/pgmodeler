include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = parsers

# The QMAKE_LFLAGS does not work properly with MingW (Windows) or Xcode (MacOSX)
# so it's necessary to pass manually all the used libraries to the linker
windows | macx {
 LIBS += $$DESTDIR/$$LIBUTILS
}

HEADERS += $$PWD/src/schemaparser.h \
           $$PWD/src/xmlparser.h

SOURCES += $$PWD/src/schemaparser.cpp \
           $$PWD/src/xmlparser.cpp

