include(../../pgmodeler.pri)

CONFIG += console
TEMPLATE = app
TARGET = pgmodeler-cli

windows:RC_FILE = res/windows_ico.qrc
windows: RCC_DIR = src/
windows: DESTDIR = $$PWD

SOURCES += src/main.cpp \
	   src/pgmodelercliapp.cpp

HEADERS += src/pgmodelercliapp.h

unix|windows: LIBS += $$LIBGUI_LIB \
		      $$LIBCANVAS_LIB \
		      $$LIBCONNECTOR_LIB \
		      $$LIBCORE_LIB \
		      $$LIBPARSERS_LIB \
		      $$LIBUTILS_LIB

INCLUDEPATH += $$LIBGUI_INC \
	       $$LIBCANVAS_INC \
	       $$LIBCONNECTOR_INC \
	       $$LIBCORE_INC \
	       $$LIBPARSERS_INC \
	       $$LIBUTILS_INC

DEPENDPATH += $$LIBGUI_ROOT \
	      $$LIBCANVAS_ROOT \
	      $$LIBCONNECTOR_ROOT \
	      $$LIBCORE_ROOT \
	      $$LIBPARSERS_ROOT \
	      $$LIBUTILS_ROOT

# Deployment settings
target.path = $$BINDIR
INSTALLS = target
