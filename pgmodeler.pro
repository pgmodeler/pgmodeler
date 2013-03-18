###########################################################
# XML / PostgreSQL variables configuration                #
#                                                         #
# PGSQL_LIB -> Full path to libpq.(so | dll | dylib)      #
# PGSQL_INC -> Root path were PgSQL includes can be found #
#                                                         #
# XML_LIB   -> Full path to libxml2.(so | dll | dylib)    #
# XML_INC   -> Root path were XML2 includes can be found  #
###########################################################
unix {
 !macx:CONFIG += link_pkgconfig
 !macx:PKGCONFIG = libpq libxml-2.0
 !macx:PGSQL_LIB = -lpq
 !macx:XML_LIB = -lxml2
}

macx {
 PGSQL_LIB = /Library/PostgreSQL/9.2/lib/libpq.dylib
 PGSQL_INC = /Library/PostgreSQL/9.2/include
 XML_INC = /usr/include/libxml2
 XML_LIB = /usr/lib/libxml2.dylib
}

windows {
 PGSQL_LIB = C:/PostgreSQL/9.2/bin/libpq.dll
 PGSQL_INC = C:/PostgreSQL/9.2/include
 XML_INC = C:/QtSDK/mingw/include
 XML_LIB = C:/QtSDK/mingw/bin/libxml2.dll
}

macx | windows {
  !exists($$PGSQL_LIB) {
    PKG_ERROR = "PostgreSQL libraries"
    VARIABLE = "PGSQL_LIB"
    VALUE = $$PGSQL_LIB
  }
}

macx | windows {
  !exists($$PGSQL_INC/libpq-fe.h) {
    PKG_ERROR = "PostgreSQL headers"
    VARIABLE = "PGSQL_INC"
    VALUE = $$PGSQL_INC
  }
}

macx | windows {
 !exists($$XML_LIB) {
    PKG_ERROR = "XML2 libraries"
    VARIABLE = "XML_LIB"
    VALUE = $$XML_LIB
 }
}

macx | windows {
 !exists($$XML_INC) {
   PKG_ERROR = "XML2 headers"
   VARIABLE = "XML_INC"
   VALUE = $$XML_INC
 }
}


macx | windows {
 !isEmpty(PKG_ERROR) {
    warning("$$PKG_ERROR were not found at \"$$VALUE\"!")
    warning("Please correct the value of $$VARIABLE and try again!")
    error("pgModeler compilation aborted.")
 }
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
macx:LIB_EXT = dylib

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

QT += core gui #qt3support
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

pgmodeler.path = $$PWD/build/
pgmodeler.files = samples schemas lang conf README.md COMPILING.md PLUGINS.md CHANGELOG.md LICENSE libpgmodeler_ui/res/imagens/pgmodeler_logo.png

unix {
 pgmodeler.files += start-pgmodeler.sh
}

macx {
 pgmodeler.files += start-pgmodeler-mac.sh
}

windows {
 pgmodeler.files += start-pgmodeler.bat
}

INSTALLS += pgmodeler

OTHER_FILES += \
    schemas/xml/permission.sch \
    schemas/sql/common/permission.sch \
    schemas/sql/9.1/permission.sch \
    schemas/sql/9.0/permission.sch \
    schemas/sql/8.4/permission.sch \
    schemas/xml/dtd/permission.dtd
