include(../pgmodeler.pro)

TEMPLATE = app
TARGET = pgmodeler

windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/
macx:QMAKE_POST_LINK+="cp -r res/Resources $$BASEDIR; \
		       cp res/Info.plist $$BASEDIR; \
		       cp res/PkgInfo $$BASEDIR; \
		       cp res/startapp $$DESTDIR"

LIBS += $$DESTDIR/$$LIBUTILS \
	$$DESTDIR/$$LIBPARSERS \
	$$DESTDIR/$$LIBPGCONNECTOR \
	$$DESTDIR/$$LIBOBJRENDERER \
	$$DESTDIR/$$LIBPGMODELER \
	$$DESTDIR/$$LIBPGMODELERUI

HEADERS += src/application.h
SOURCES += src/main.cpp \
    src/application.cpp


target.path = $$BINDESTDIR
INSTALLS = target