include(../../pgmodeler.pri)

TEMPLATE = app
TARGET = pgmodeler

windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/
windows: DESTDIR = $$PWD

HEADERS += src/pgmodelerapp.h

SOURCES += src/main.cpp \
	src/pgmodelerapp.cpp

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

macx {
  macdeps.files = $$PWD/res/Resources $$PWD/res/Info.plist $$PWD/res/PkgInfo
  macdeps.path = $$PREFIX

  macscript.files = $$PWD/res/startapp
  macscript.path = $$BINDIR

  INSTALLS += macdeps macscript
}


