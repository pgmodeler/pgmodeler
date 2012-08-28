CONFIG += ordered
TEMPLATE = subdirs

unix {
 GLOBAL_INCLUDES = .\
                   /usr/local/pgsql/include \
                   /usr/include \
                   /usr/include/libxml2


 GLOBAL_LIBS =  /usr/local/pgsql/lib/libpq.so \
                -lxml2

 #LIBUTIL = libutil.so
 #LIBCONEXBD = libconexbd.so
 #LIBPGMODELER = libpgmodeler.so
 #LIBOBJRENDERER = libobjrenderer.so
 #LIBPARSERS = libparsers.so
}

windows {
 GLOBAL_INCLUDES = .\
                   "C:/PostgreSQL/9.1/include/" \
                   "C:/QtSDK/mingw/include"

 GLOBAL_LIBS = "C:/PostgreSQL/9.1/lib/libpq.dll" \
               "C:/QtSDK/mingw/bin/libxml2.dll"

 QMAKE_LFLAGS=-Wl,-enable-auto-import

 #LIBUTIL = util.dll
 #LIBCONEXBD = conexbd.dll
 #LIBPGMODELER = pgmodeler.dll
 #LIBOBJRENDERER = objrender.dll
 #LIBPARSERS = parsers.dll
}

INCLUDEPATH = $${GLOBAL_INCLUDES} \
              $$PWD/libutil/src \
              $$PWD/libconexbd/src \
              $$PWD/libparsers/src \
              $$PWD/libpgmodeler/src \
              $$PWD/libobjrenderer/src \
              $$PWD/pgmodeler_ui/src

LIBS = $${GLOBAL_LIBS}

#SUBDIRS = libutil \
#          libconexbd \
#          libparsers \
#          libpgmodeler \
#          libobjrenderer \
#          pgmodeler_ui

SUBDIRS = pgmodeler_ui


sources.files = models schemas lang conf README.md COMPILING.md LICENSE
sources.path = .
INSTALLS += sources

