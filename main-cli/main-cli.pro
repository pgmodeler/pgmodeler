include(../pgmodeler.pro)

CONFIG += console
TEMPLATE = app
TARGET = pgmodeler-cli
INCLUDEPATH += ../main/src

windows:RC_FILE=../main/res/windows_ico.qrc
windows:RCC_DIR=src/

!macx {
 # Check if BINDESTDIR points to another location other than DESTDIR
 # in this case the INSTALLS will be used
 !equals(BINDESTDIR, $$DESTDIR) {
  target.path = $$BINDESTDIR
  INSTALLS = target
 }

 LIBS += $$DESTDIR/$$LIBUTILS \
         $$DESTDIR/$$LIBPARSERS \
         $$DESTDIR/$$LIBPGCONNECTOR \
         $$DESTDIR/$$LIBOBJRENDERER \
         $$DESTDIR/$$LIBPGMODELER \
         $$DESTDIR/$$LIBPGMODELERUI
}

macx {
 DESTDIR=$$BINDESTDIR
 LIBS += $$LIBDESTDIR/$$LIBUTILS \
         $$LIBDESTDIR/$$LIBPARSERS \
         $$LIBDESTDIR/$$LIBPGCONNECTOR \
         $$LIBDESTDIR/$$LIBOBJRENDERER \
         $$LIBDESTDIR/$$LIBPGMODELER \
         $$LIBDESTDIR/$$LIBPGMODELERUI
}

SOURCES += src/main.cpp \
	   src/pgmodelercli.cpp

HEADERS += src/pgmodelercli.h
