include(../pgmodeler.pro)

TEMPLATE = app
TARGET = pgmodeler

windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/

LIBS += $$DESTDIR/$$LIBUTILS \
	$$DESTDIR/$$LIBPARSERS \
	$$DESTDIR/$$LIBDBCONNECT \
	$$DESTDIR/$$LIBOBJRENDERER \
	$$DESTDIR/$$LIBPGMODELER \
	$$DESTDIR/$$LIBPGMODELERUI

HEADERS += $$PWD/src/application.h
SOURCES += $$PWD/src/main.cpp
