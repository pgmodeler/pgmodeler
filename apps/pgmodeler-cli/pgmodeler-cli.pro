include(../apps.pri)

CONFIG += console
TEMPLATE = app
TARGET = pgmodeler-cli

windows:RC_FILE = res/windows_ico.qrc
windows: RCC_DIR = src/
windows: DESTDIR = $$PWD

SOURCES += src/main.cpp
#src/compat/compatns.cpp \
#src/pgmodelercliapp.cpp \
#src/compat/view.cpp \
#src/compat/reference.cpp

#HEADERS += src/pgmodelercliapp.h \
#src/compat/compatns.h \
#src/compat/view.h \
#src/compat/reference.h

# Deployment settings
target.path = $$BINDIR
INSTALLS = target
