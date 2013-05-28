include(../pgmodeler.pro)

TEMPLATE = app
TARGET = crashhandler

macx:ICON=res/crashhandler.icns
macx:QMAKE_INFO_PLIST=res/Info.plist

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
