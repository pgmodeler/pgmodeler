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

# Check if BINDESTDIR points to another location other than DESTDIR
# in this case the INSTALLS will be used
!equals(BINDESTDIR, $$DESTDIR) {
 target.path = $$BINDESTDIR
 INSTALLS = target
}
