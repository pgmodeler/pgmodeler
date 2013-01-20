###########################################################
# XML / PostgreSQL variables configuration                #
#                                                         #
# PGSQL_LIB -> Full path to libpq.(so | dll)              #
# PGSQL_INC -> Root path were PgSQL includes can be found #
#                                                         #
# XML_LIB   -> Full path to libxml2.(so | dll)            #
# XML_INC   -> Root path were XML2 includes can be found  #
###########################################################
unix {
 PGSQL_LIB = /usr/lib64/libpq.so
 PGSQL_INC = /usr/include
 XML_INC = /usr/include/libxml2
 XML_LIB = /usr/lib64/libxml2.so
}

windows {
 PGSQL_LIB = C:/PostgreSQL/9.2/bin/libpq.dll
 PGSQL_INC = C:/PostgreSQL/9.2/include
 XML_INC = C:/QtSDK/mingw/include
 XML_LIB = C:/QtSDK/mingw/bin/libxml2.dll
}

!exists($$PGSQL_LIB) {
  PKG_ERROR = "PostgreSQL libraries"
  VARIABLE = "PGSQL_LIB"
  VALUE = $$PGSQL_LIB
}

!exists($$PGSQL_INC/libpq-fe.h) {
  PKG_ERROR = "PostgreSQL headers"
  VARIABLE = "PGSQL_INC"
  VALUE = $$PGSQL_INC
}

!exists($$XML_LIB) {
  PKG_ERROR = "XML2 libraries"
  VARIABLE = "XML_LIB"
  VALUE = $$XML_LIB
}

!exists($$XML_INC) {
  PKG_ERROR = "XML2 headers"
  VARIABLE = "XML_INC"
  VALUE = $$XML_INC
}

!isEmpty(PKG_ERROR) {
  warning("$$PKG_ERROR were not found at \"$$VALUE\"!")
  warning("Please correct the value of $$VARIABLE and try again!")
  error("pgModeler compilation aborted.")
}

###########################
# Main variables settings #
###########################
CONFIG += ordered qt stl rtti exceptions warn_on
unix:CONFIG += x11
windows:CONFIG += windows
unix:LIB_PREFIX = lib
unix:LIB_EXT = so
windows:LIB_EXT = dll

SUBDIRS = libutil \
          libparsers \
          libdbconnect \
          libpgmodeler \
          libobjrenderer \
          libpgmodeler_ui \
          crashhandler \
          main \
          plugins/dummyplugin


LIBUTIL=$${LIB_PREFIX}util.$${LIB_EXT}
LIBPARSERS=$${LIB_PREFIX}parsers.$${LIB_EXT}
LIBDBCONNECT=$${LIB_PREFIX}dbconnect.$${LIB_EXT}
LIBPGMODELER=$${LIB_PREFIX}pgmodeler.$${LIB_EXT}
LIBOBJRENDERER=$${LIB_PREFIX}objrenderer.$${LIB_EXT}
LIBPGMODELERUI=$${LIB_PREFIX}pgmodeler_ui.$${LIB_EXT}

QT += core gui qt3support
TEMPLATE = subdirs
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src

INCLUDEPATH += $$XML_INC \
               $$PGSQL_INC \
               $$PWD/libutil/src \
               $$PWD/libdbconnect/src \
               $$PWD/libparsers/src \
               $$PWD/libpgmodeler/src \
               $$PWD/libobjrenderer/src \
               $$PWD/libpgmodeler_ui/src

LIBS = $$XML_LIB $$PGSQL_LIB

sources.files = samples schemas lang conf README.md COMPILING.md PLUGINS.md CHANGELOG.md LICENSE libpgmodeler_ui/res/imagens/pgmodeler_logo.png
unix:sources.files += start-pgmodeler.sh
windows:sources.files += start-pgmodeler.bat
sources.path = build/
INSTALLS += sources
