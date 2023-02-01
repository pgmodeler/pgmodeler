include(../apps.pri)

TEMPLATE = app
TARGET = pgmodeler-ch

windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/

SOURCES += src/main.cpp \
           src/crashhandlerform.cpp

HEADERS += src/crashhandlerform.h

# Deployment settings
target.path = $$PRIVATEBINDIR
INSTALLS = target
