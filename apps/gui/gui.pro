include(../../pgmodeler.pri)

TEMPLATE = app
TARGET = pgmodeler

windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/
windows: DESTDIR = $$PWD

HEADERS += src/pgmodelerapp.h

SOURCES += src/main.cpp \
	src/pgmodelerapp.cpp

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

macx {
  macdeps.files = $$PWD/res/Resources $$PWD/res/Info.plist $$PWD/res/PkgInfo
  macdeps.path = $$PREFIX

  macscript.files = $$PWD/res/startapp
  macscript.path = $$BINDIR

  INSTALLS += macdeps macscript
}

# Print the current build settins (see pgmodeler.pri)
printBuildDetails()
