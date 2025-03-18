include(../apps.pri)

TEMPLATE = app
TARGET = pgmodeler-ch

isEqual(PRIVATE_PLUGINS, true) {
  windows:RC_FILE=$$PRIV_RES_FOLDER/$$TARGET/windows_ico.qrc
}

SOURCES += src/main.cpp \
           src/crashhandlerform.cpp

HEADERS += src/crashhandlerform.h

# Deployment settings
target.path = $$PRIVATEBINDIR
INSTALLS = target
