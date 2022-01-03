include(../apps.pri)

TEMPLATE = app
TARGET = pgmodeler

unix:LIBS += $$QMAKE_LIBS_EXECINFO
windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/
windows: DESTDIR = $$PWD

HEADERS += src/pgmodelerapp.h

SOURCES += src/main.cpp \
	src/pgmodelerapp.cpp

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


