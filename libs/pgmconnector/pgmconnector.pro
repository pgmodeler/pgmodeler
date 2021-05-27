include(../../pgmodeler.pri)

TEMPLATE = lib
TARGET = pgmconnector
windows: DESTDIR = $$PWD

HEADERS += src/resultset.h \
	   src/connection.h \
	   src/catalog.h

SOURCES += src/resultset.cpp \
	   src/connection.cpp \
	   src/catalog.cpp

unix|windows: LIBS += $$PGSQL_LIB \
		      $$PGMCORE_LIB \
		      $$PGMPARSERS_LIB \
		      $$PGMUTILS_LIB

INCLUDEPATH += $$PGMCORE_INC \
	       $$PGMPARSERS_INC \
	       $$PGMUTILS_INC

DEPENDPATH += $$PGMCORE_ROOT \
	      $$PGMPARSERS_ROOT \
	      $$PGMUTILS_ROOT

# Deployment settings
target.path = $$PRIVATELIBDIR
INSTALLS = target
