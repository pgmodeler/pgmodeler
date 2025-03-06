include(../apps.pri)

TEMPLATE = app
TARGET = pgmodeler-se

isEqual(PRIVATE_PLUGINS, true) {
  windows:RC_FILE=$$PRIV_RES_FOLDER/$$TARGET/windows_ico.qrc
}

#isEqual(PRIVATE_PLUGINS, false) {
#  windows:RC_FILE=res/windows_ico.qrc
#}

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
