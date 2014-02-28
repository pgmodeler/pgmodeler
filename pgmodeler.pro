############################################################
# XML / PostgreSQL variables configuration                 #
#                                                          #
# PGSQL_LIB -> Full path to libpq.(so | dll | dylib)       #
# PGSQL_INC -> Root path where PgSQL includes can be found #
#                                                          #
# XML_LIB   -> Full path to libxml2.(so | dll | dylib)     #
# XML_INC   -> Root path where XML2 includes can be found  #
############################################################
#cache()

unix {
 !macx:CONFIG += link_pkgconfig
 !macx:PKGCONFIG = libpq libxml-2.0
 !macx:PGSQL_LIB = -lpq
 !macx:XML_LIB = -lxml2
}

macx {
 PGSQL_LIB = /Library/PostgreSQL/9.3/lib/libpq.dylib
 PGSQL_INC = /Library/PostgreSQL/9.3/include
 XML_INC = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/usr/include/libxml2
 XML_LIB = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk/usr/lib/libxml2.dylib
}

windows {
 PGSQL_LIB = C:/PostgreSQL/9.2/bin/libpq.dll
 PGSQL_INC = C:/PostgreSQL/9.2/include
 XML_INC = C:/Qt/Qt5.2.0/5.2.0/mingw48_32/include
 XML_LIB = C:/Qt/Qt5.2.0/5.2.0/mingw48_32/bin/libxml2.dll
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
CONFIG += ordered qt stl rtti exceptions warn_on c++11

#Additional configs on unix / windows
unix:CONFIG += x11
windows:CONFIG += windows

macx:CONFIG-=app_bundle

#Libraries extension and preffix for each platform
unix:LIB_PREFIX = lib
unix:LIB_EXT = so
windows:LIB_EXT = dll
macx:LIB_EXT = dylib

SUBDIRS = libutils \
          libparsers \
          libpgmodeler \
          libpgconnector \
          libobjrenderer \
          libpgmodeler_ui \
          crashhandler \
          main-cli \
          main \
          plugins/dummy \
          plugins/xml2object

#Include the tests subproject only on debug mode
CONFIG(debug, debug|release):SUBDIRS+=tests

QT += core widgets printsupport
TEMPLATE = subdirs
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src

macx {
 BASEDIR = $$PWD/build/pgmodeler.app/Contents
 DESTDIR = $$BASEDIR/MacOS #Where the compiled executables are stored
 LIBDESTDIR = $$BASEDIR/Frameworks #Where the compiled libs are stored
 RESDESTDIR = $$DESTDIR
}

# User can pass additional config params to specify custom output directory as follow:
#
# BINDIR = where any generated binary will be installed.
# LIBDIR = where any generated library will be installed.
# RESDIR = where resource files like conf, lang, samples and schema directories will be installed.
# DOCDIR = where doc files will be installed.
#
# By default, when not specifying any of above options, the compilation will generate files
# under ./build.
#
linux|windows {
 DESTDIR = $$PWD/build

 BINDESTDIR = $$DESTDIR
 defined(BINDIR, var): BINDESTDIR=$$BINDIR

 #On Linux and Windows the compiled libs resides on the same executable's dir (by default)
 LIBDESTDIR = $$DESTDIR
 defined(LIBDIR, var): LIBDESTDIR=$$LIBDIR

 #On Linux and Windows the resource files (conf, schemas, etc) resides on the same executable's dir (by default)
 RESDESTDIR = $$DESTDIR
 defined(RESDIR, var): RESDESTDIR=$$RESDIR
 
 DOCDESTDIR = $$DESTDIR
 defined(DOCDIR, var): DOCDESTDIR=$$DOCDIR
}

#Creating the project's libraries names based upon the running OS
LIBUTILS=$${LIB_PREFIX}utils.$${LIB_EXT}
LIBPARSERS=$${LIB_PREFIX}parsers.$${LIB_EXT}
LIBPGCONNECTOR=$${LIB_PREFIX}pgconnector.$${LIB_EXT}
LIBPGMODELER=$${LIB_PREFIX}pgmodeler.$${LIB_EXT}
LIBOBJRENDERER=$${LIB_PREFIX}objrenderer.$${LIB_EXT}
LIBPGMODELERUI=$${LIB_PREFIX}pgmodeler_ui.$${LIB_EXT}

INCLUDEPATH += $$XML_INC \
               $$PGSQL_INC \
               $$PWD/libutils/src \
               $$PWD/libpgconnector/src \
               $$PWD/libparsers/src \
               $$PWD/libpgmodeler/src \
               $$PWD/libobjrenderer/src \
               $$PWD/libpgmodeler_ui/src \
               $$PWD/main/src


#Deployment configurations
pgmodeler_data.files = samples schemas lang conf
pgmodeler_data.path = $$RESDESTDIR

pgmodeler_doc.files = README.md CHANGELOG.md LICENSE
pgmodeler_doc.path = $$DOCDESTDIR

unix {
!macx:pgmodeler_data.files += pgmodeler.vars
}

# deploy binaries
# pgmodeler.path = $$BINDESTDIR

INSTALLS += pgmodeler_data pgmodeler_doc
