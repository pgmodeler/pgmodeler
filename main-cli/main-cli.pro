include(../pgmodeler.pro)

CONFIG += console
TEMPLATE = app
TARGET = pgmodeler-cli
INCLUDEPATH += ../main/src

macx:ICON=res/pgmodeler-cli.icns
macx:QMAKE_INFO_PLIST=res/Info.plist

LIBS += $$DESTDIR/$$LIBUTILS \
	$$DESTDIR/$$LIBPARSERS \
	$$DESTDIR/$$LIBDBCONNECT \
	$$DESTDIR/$$LIBOBJRENDERER \
	$$DESTDIR/$$LIBPGMODELER \
	$$DESTDIR/$$LIBPGMODELERUI

SOURCES += src/main.cpp \
	   src/pgmodelercli.cpp

HEADERS += src/pgmodelercli.h
