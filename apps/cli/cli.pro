include(../../pgmodeler.pri)

CONFIG += console
TEMPLATE = app
TARGET = pgmodeler-cli
INCLUDEPATH += ../gui/src

windows:RC_FILE=../gui/res/windows_ico.qrc
windows: RCC_DIR=src/
windows: DESTDIR = $$PWD

SOURCES += src/main.cpp \
	   src/pgmodelercliapp.cpp

HEADERS += src/pgmodelercliapp.h

unix|windows: LIBS += $$PGMUI_LIB \
		      $$PGMCANVAS_LIB \
		      $$PGMCONNECTOR_LIB \
		      $$PGMCORE_LIB \
		      $$PGMPARSERS_LIB \
		      $$PGMUTILS_LIB

INCLUDEPATH += $$PGMUI_INC \
	       $$PGMCANVAS_INC \
	       $$PGMCONNECTOR_INC \
	       $$PGMCORE_INC \
	       $$PGMPARSERS_INC \
	       $$PGMUTILS_INC

DEPENDPATH += $$PGMUI_ROOT \
	      $$PGMCANVAS_ROOT \
	      $$PGMCONNECTOR_ROOT \
	      $$PGMCORE_ROOT \
	      $$PGMPARSERS_ROOT \
	      $$PGMUTILS_ROOT

# Deployment settings
target.path = $$BINDIR
INSTALLS = target
