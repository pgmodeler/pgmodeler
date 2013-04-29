include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = dbconnect
OBJECTS_DIR = obj

# The QMAKE_LFLAGS does not work properly with MingW (Windows) or Xcode (MacOSX)
# so it's necessary to pass manually all the used libraries to the linker
windows | macx {
 LIBS += $$DESTDIR/$$LIBUTILS
}

HEADERS += $$PWD/src/dbconnection.h \
           $$PWD/src/resultset.h

SOURCES += $$PWD/src/dbconnection.cpp \
           $$PWD/src/resultset.cpp
