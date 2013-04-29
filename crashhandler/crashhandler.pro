include(../pgmodeler.pro)

TEMPLATE = app
TARGET = crashhandler

LIBS += $$DESTDIR/$$LIBUTILS \
	$$DESTDIR/$$LIBPARSERS \
	$$DESTDIR/$$LIBDBCONNECT \
	$$DESTDIR/$$LIBOBJRENDERER \
	$$DESTDIR/$$LIBPGMODELER \
	$$DESTDIR/$$LIBPGMODELERUI

FORMS += $$PWD/ui/crashhandler.ui

SOURCES += $$PWD/src/main.cpp \
           $$PWD/src/crashhandler.cpp

HEADERS += $$PWD/src/crashhandler.h
