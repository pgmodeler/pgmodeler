include(../apps.pri)

CONFIG += console
TEMPLATE = app
TARGET = pgmodeler-cli

windows:RC_FILE = res/windows_ico.qrc
windows: RCC_DIR = src/
windows: DESTDIR = $$PWD

SOURCES += src/main.cpp

# Deployment settings
target.path = $$BINDIR
INSTALLS = target
