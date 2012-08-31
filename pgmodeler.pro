CONFIG += ordered
TEMPLATE = subdirs

unix {
 GLOBAL_INCLUDES = .\
                   /usr/local/pgsql/include \
                   /usr/include \
                   /usr/include/libxml2


 GLOBAL_LIBS =  /usr/local/pgsql/lib/libpq.so \
                -lxml2

 LIBUTIL=libutil.so
 LIBPARSERS=libparsers.so
 LIBCONEXBD=libconexbd.so
 LIBPGMODELER=libpgmodeler.so
 LIBOBJRENDERER=libobjrenderer.so
 LIBPGMODELERUI=libpgmodeler_ui.so
}

windows {
 GLOBAL_INCLUDES = .\
                   "C:/PostgreSQL/9.1/include/" \
                   "C:/QtSDK/mingw/include"

 GLOBAL_LIBS = "C:/PostgreSQL/9.1/lib/libpq.dll" \
               "C:/QtSDK/mingw/bin/libxml2.dll"

 QMAKE_LFLAGS=-Wl,-enable-auto-import

 LIBUTIL=util.dll
 LIBPARSERS=parsers.dll
 LIBCONEXBD=conexbd.dll
 LIBPGMODELER=pgmodeler.dll
 LIBOBJRENDERER=objrenderer.dll
 LIBPGMODELERUI=pgmodeler_ui.dll
}

INCLUDEPATH = $${GLOBAL_INCLUDES} \
              $$PWD/libutil/src \
              $$PWD/libconexbd/src \
              $$PWD/libparsers/src \
              $$PWD/libpgmodeler/src \
              $$PWD/libobjrenderer/src \
              $$PWD/libpgmodeler_ui/src

LIBS = $${GLOBAL_LIBS}

SUBDIRS = libutil \
          libparsers \
          libconexbd \
          libpgmodeler \
          libobjrenderer \
          libpgmodeler_ui \
          main \
          plugins/dummyplugin

sources.files = models schemas lang conf README.md COMPILING.md PLUGINS.md LICENSE
sources.path = .
INSTALLS += sources

