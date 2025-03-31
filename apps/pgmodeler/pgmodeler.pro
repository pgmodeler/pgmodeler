include(../apps.pri)

TEMPLATE = app
TARGET = pgmodeler

isEqual(PRIVATE_PLUGINS, true) {
  windows:RC_FILE=$$PRIV_RES_FOLDER/$$TARGET/windows_ico.qrc
}

HEADERS += src/pgmodelerapp.h

SOURCES += src/main.cpp \
src/pgmodelerapp.cpp

# Deployment settings
target.path = $$BINDIR
INSTALLS = target

macx {
	macdeps.files = $$PWD/res/Info.plist $$PWD/res/PkgInfo

	isEqual(PRIVATE_PLUGINS, true) {
		macdeps.files += $$PRIV_RES_FOLDER/$$TARGET/Resources
	}

	isEqual(PRIVATE_PLUGINS, false) {
		macdeps.files += $$PWD/res/Resources
	}

	macdeps.path = $$PREFIX

  macscript.files = $$PWD/res/startapp
  macscript.path = $$BINDIR

  INSTALLS += macdeps macscript
}


