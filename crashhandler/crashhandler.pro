include(../pgmodeler.pro)

TEMPLATE = app
TARGET = crashhandler

LIBS += $$DESTDIR/$$LIBUTILS \
	$$DESTDIR/$$LIBPARSERS \
	$$DESTDIR/$$LIBDBCONNECT \
	$$DESTDIR/$$LIBOBJRENDERER \
	$$DESTDIR/$$LIBPGMODELER \
	$$DESTDIR/$$LIBPGMODELERUI

FORMS += ui/crashhandler.ui

SOURCES += src/main.cpp \
	   src/crashhandler.cpp

HEADERS += src/crashhandler.h
