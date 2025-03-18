include(../apps.pri)

CONFIG += console
TEMPLATE = app
TARGET = pgmodeler-cli

isEqual(PRIVATE_PLUGINS, true) {
  windows:RC_FILE=$$PRIV_RES_FOLDER/$$TARGET/windows_ico.qrc
}

SOURCES += src/main.cpp

# Deployment settings
target.path = $$BINDIR
INSTALLS = target
