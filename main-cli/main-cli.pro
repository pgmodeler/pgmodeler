include(../pgmodeler.pro)

TEMPLATE = app
TARGET = pgmodeler-cli
INCLUDEPATH += ../main/src

LIBS += $$DESTDIR/$$LIBUTILS \
	$$DESTDIR/$$LIBPARSERS \
	$$DESTDIR/$$LIBDBCONNECT \
	$$DESTDIR/$$LIBOBJRENDERER \
	$$DESTDIR/$$LIBPGMODELER \
	$$DESTDIR/$$LIBPGMODELERUI

SOURCES += src/main.cpp \
	   src/pgmodelercli.cpp

HEADERS += src/pgmodelercli.h
