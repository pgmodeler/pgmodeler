include(../pgmodeler.pro)

TEMPLATE = app
TARGET = pgmodeler

windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/
macx:QMAKE_POST_LINK+="cp -r res/Resources $$BASEDIR; \
		       cp res/Info.plist $$BASEDIR; \
		       cp res/PkgInfo $$BASEDIR; \
		       cp res/startapp $$DESTDIR"

LIBS += $$LIBDESTDIR/$$LIBUTILS \
	$$LIBDESTDIR/$$LIBPARSERS \
	$$LIBDESTDIR/$$LIBDBCONNECT \
	$$LIBDESTDIR/$$LIBOBJRENDERER \
	$$LIBDESTDIR/$$LIBPGMODELER \
	$$LIBDESTDIR/$$LIBPGMODELERUI

HEADERS += src/application.h
SOURCES += src/main.cpp


