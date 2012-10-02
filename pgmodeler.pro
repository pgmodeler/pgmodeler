CONFIG += ordered qt stl rtti exceptions

contains(CONFIG, debug) {
 CONFIG += warn_on
}

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libxml-2.0 libpq
unix:CONFIG += x11
windows:CONFIG += windows
unix:LIB_PREFIX = lib
unix:LIB_EXT = so
windows:LIB_EXT = dll

isEmpty(BUILDCONF): BUILDCONF += shared

contains(BUILDCONF, shared) {
 SUBDIRS = libutil \
           libparsers \
           libconexbd \
           libpgmodeler \
           libobjrenderer \
           libpgmodeler_ui \
           main \
           plugins/dummyplugin
}

contains(BUILDCONF, static) {
 SUBDIRS = main
           #plugins/dummyplugin
}


LIBUTIL=$${LIB_PREFIX}util.$${LIB_EXT}
LIBPARSERS=$${LIB_PREFIX}parsers.$${LIB_EXT}
LIBCONEXBD=$${LIB_PREFIX}conexbd.$${LIB_EXT}
LIBPGMODELER=$${LIB_PREFIX}pgmodeler.$${LIB_EXT}
LIBOBJRENDERER=$${LIB_PREFIX}objrenderer.$${LIB_EXT}
LIBPGMODELERUI=$${LIB_PREFIX}pgmodeler_ui.$${LIB_EXT}

QT += core gui qt3support
TEMPLATE = subdirs
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src

INCLUDEPATH += $$PWD/libutil/src \
               $$PWD/libconexbd/src \
               $$PWD/libparsers/src \
               $$PWD/libpgmodeler/src \
               $$PWD/libobjrenderer/src \
               $$PWD/libpgmodeler_ui/src
unix:LIBS = -lxml2 -lpq

windows:INCLUDEPATH += C:/mingw/include C:/PostgreSQL/9.2/include
windows:LIBS = C:/mingw/bin/libxml2.dll C:/PostgreSQL/9.2/bin/libpq.dll

sources.files = samples schemas lang conf README.md COMPILING.md PLUGINS.md LICENSE libpgmodeler_ui/res/imagens/pgmodeler_logo.png
unix:sources.files += start-pgmodeler.sh
windows:sources.files += start-pgmodeler.bat
sources.path = build/
INSTALLS += sources
