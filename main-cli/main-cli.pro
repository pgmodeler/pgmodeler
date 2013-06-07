include(../pgmodeler.pro)

CONFIG += console
TEMPLATE = app
TARGET = pgmodeler-cli
INCLUDEPATH += ../main/src

windows:RC_FILE=../main/res/windows_ico.qrc
windows:RCC_DIR=src/

LIBS += $$LIBDESTDIR/$$LIBUTILS \
	$$LIBDESTDIR/$$LIBPARSERS \
	$$LIBDESTDIR/$$LIBDBCONNECT \
	$$LIBDESTDIR/$$LIBOBJRENDERER \
	$$LIBDESTDIR/$$LIBPGMODELER \
	$$LIBDESTDIR/$$LIBPGMODELERUI

SOURCES += src/main.cpp \
	   src/pgmodelercli.cpp

HEADERS += src/pgmodelercli.h
