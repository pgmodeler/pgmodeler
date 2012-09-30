CONFIG += ordered qt warn_on uitools uic4 stl rtti exceptions
unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libxml-2.0 libpq
unix:CONFIG += x11
windows:CONFIG += windows

QT += core gui qt3support
TEMPLATE = subdirs
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src

unix:LIB_EXT = so
unix:LIB_PREFIX = lib
windows:SHARED_LIBS = dll
windows:QMAKE_LFLAGS=-Wl,-enable-auto-import

LIBUTIL=$${LIB_PREFIX}util.$${LIB_EXT}
LIBPARSERS=$${LIB_PREFIX}parsers.$${LIB_EXT}
LIBCONEXBD=$${LIB_PREFIX}conexbd.$${LIB_EXT}
LIBPGMODELER=$${LIB_PREFIX}pgmodeler.$${LIB_EXT}
LIBOBJRENDERER=$${LIB_PREFIX}objrenderer.$${LIB_EXT}
LIBPGMODELERUI=$${LIB_PREFIX}pgmodeler_ui.$${LIB_EXT}

INCLUDEPATH = $${GLOBAL_INCLUDES} \
              $$PWD/libutil/src \
              $$PWD/libconexbd/src \
              $$PWD/libparsers/src \
              $$PWD/libpgmodeler/src \
              $$PWD/libobjrenderer/src \
              $$PWD/libpgmodeler_ui/src

LIBS = -lxml2 -lpq

SUBDIRS = libutil \
          libparsers \
          libconexbd \
          libpgmodeler \
          libobjrenderer \
          libpgmodeler_ui \
          main \
          plugins/dummyplugin

sources.files = samples schemas lang conf README.md COMPILING.md PLUGINS.md LICENSE libpgmodeler_ui/res/imagens/pgmodeler_logo.png
unix:sources.files += start-pgmodeler.sh
windows:sources.files += start-pgmodeler.bat
sources.path = build/
INSTALLS += sources
