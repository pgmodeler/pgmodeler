include(../apps.pri)

TEMPLATE = app
TARGET = pgmodeler

#unix:LIBS += $$QMAKE_LIBS_EXECINFO

isEqual(PRIVATE_PLUGINS, true) {
  windows:RC_FILE=$$PRIV_RES_FOLDER/$$TARGET/windows_ico.qrc
}

#isEqual(PRIVATE_PLUGINS, false) {
#  windows:RC_FILE=res/windows_ico.qrc
#}

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


