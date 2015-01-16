include(../pgmodeler.pri)

TEMPLATE = lib
TARGET = pgconnector
OBJECTS_DIR = obj

HEADERS += src/resultset.h \
	   src/connection.h \
	   src/catalog.h

SOURCES += src/resultset.cpp \
	   src/connection.cpp \
	   src/catalog.cpp

unix|win32: LIBS += $$PGSQL_LIB\
                    -L$$OUT_PWD/../libpgmodeler/ -lpgmodeler \
                    -L$$OUT_PWD/../libparsers/ -lparsers \
                    -L$$OUT_PWD/../libutils/ -lutils

INCLUDEPATH += $$PWD/../libpgmodeler/src \
               $$PWD/../libparsers/src \
               $$PWD/../libutils/src

DEPENDPATH += $$PWD/../libpgmodeler \
              $$PWD/../libparsers \
              $$PWD/../libutils

# Deployment settings
target.path = $$PRIVATELIBDIR
INSTALLS = target
