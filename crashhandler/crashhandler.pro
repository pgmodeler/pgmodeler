include(../pgmodeler.pro)

TEMPLATE = app
TARGET = crashhandler

windows:RC_FILE=../main/res/windows_ico.qrc
windows:RCC_DIR=src/

LIBS += $$LIBDESTDIR/$$LIBUTILS \
	$$LIBDESTDIR/$$LIBPARSERS \
	$$LIBDESTDIR/$$LIBDBCONNECT \
	$$LIBDESTDIR/$$LIBOBJRENDERER \
	$$LIBDESTDIR/$$LIBPGMODELER \
	$$LIBDESTDIR/$$LIBPGMODELERUI

FORMS += ui/crashhandler.ui

SOURCES += src/main.cpp \
	   src/crashhandler.cpp

HEADERS += src/crashhandler.h
