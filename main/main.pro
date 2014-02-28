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

# Check if BINDESTDIR points to another location other than DESTDIR
# in this case the INSTALLS will be used
!equals(BINDESTDIR, $$DESTDIR) {
 target.path = $$BINDESTDIR
 INSTALLS = target
}
