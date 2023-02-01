include(../apps.pri)

TEMPLATE = app
TARGET = pgmodeler-se

windows:RC_FILE=res/windows_ico.qrc
windows:RCC_DIR=src/

SOURCES += src/main.cpp \
    src/aboutsewidget.cpp \
	    src/schemaeditorform.cpp \
	    src/sourceeditorwidget.cpp \
	    ../pgmodeler/src/pgmodelerapp.cpp \

FORMS += \
	 ui/aboutsewidget.ui \
	 ui/schemaeditorform.ui \
	 ui/sourceeditorwidget.ui

HEADERS += \
	   src/aboutsewidget.h \
	   src/schemaeditorform.h \
	   src/sourceeditorwidget.h

INCLUDEPATH += ../pgmodeler/src

DEPENDPATH += ../pgmodeler

# Deployment settings
target.path = $$PRIVATEBINDIR
INSTALLS = target

# Print the current build settins (see pgmodeler.pri)
printBuildDetails()
