include(../pgmodeler.pro)

TEMPLATE = app
TARGET = pgmodeler

windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/
macx:ICON=res/pgmodeler.icns
macx:QMAKE_INFO_PLIST=res/Info.plist

LIBS += $$DESTDIR/$$LIBUTILS \
	$$DESTDIR/$$LIBPARSERS \
	$$DESTDIR/$$LIBDBCONNECT \
	$$DESTDIR/$$LIBOBJRENDERER \
	$$DESTDIR/$$LIBPGMODELER \
	$$DESTDIR/$$LIBPGMODELERUI

HEADERS += src/application.h
SOURCES += src/main.cpp


