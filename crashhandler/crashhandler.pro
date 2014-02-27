include(../pgmodeler.pro)

TEMPLATE = app
TARGET = pgmodeler-ch

windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/

LIBS += $$DESTDIR/$$LIBUTILS \
	$$DESTDIR/$$LIBPARSERS \
	$$DESTDIR/$$LIBPGCONNECTOR \
	$$DESTDIR/$$LIBOBJRENDERER \
	$$DESTDIR/$$LIBPGMODELER \
	$$DESTDIR/$$LIBPGMODELERUI

FORMS += ui/crashhandler.ui

SOURCES += src/main.cpp \
	   src/crashhandler.cpp

HEADERS += src/crashhandler.h

target.path = $$BINDESTDIR
INSTALLS = target