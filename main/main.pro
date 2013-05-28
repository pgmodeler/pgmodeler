include(../pgmodeler.pro)

TEMPLATE = app
TARGET = pgmodeler

windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/
macx:QMAKE_POST_LINK+="cp -r ../res/* $$BASEDIR; cp ../startapp $$DESTDIR"

LIBS += $$DESTDIR/$$LIBUTILS \
	$$DESTDIR/$$LIBPARSERS \
	$$DESTDIR/$$LIBDBCONNECT \
	$$DESTDIR/$$LIBOBJRENDERER \
	$$DESTDIR/$$LIBPGMODELER \
	$$DESTDIR/$$LIBPGMODELERUI

HEADERS += src/application.h
SOURCES += src/main.cpp


