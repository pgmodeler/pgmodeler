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

SOURCES += $$PWD/src/main.cpp \
	   $$PWD/src/pgmodelercli.cpp

HEADERS += $$PWD/src/pgmodelercli.h
