include(../pgmodeler.pro)

TEMPLATE = lib
TARGET = dbconnect
OBJECTS_DIR = obj

LIBS += $$DESTDIR/$$LIBUTILS

HEADERS += $$PWD/src/dbconnection.h \
           $$PWD/src/resultset.h

SOURCES += $$PWD/src/dbconnection.cpp \
           $$PWD/src/resultset.cpp
