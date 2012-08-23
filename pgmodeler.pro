CONFIG += ordered

unix {
 GLOBAL_INCLUDES = .\
                   /usr/local/pgsql/include \
                   /usr/include \
                   /usr/include/libxml2


 GLOBAL_LIBS =  /usr/local/pgsql/lib/libpq.so \
                -lxml2
}

windows {
 GLOBAL_INCLUDES = .\
                   "C:/PostgreSQL/9.1/include/" \
                   "C:/QtSDK/mingw/include"

 GLOBAL_LIBS = "C:/PostgreSQL/9.1/lib/libpq.dll" \
               "C:/QtSDK/mingw/bin/libxml2.dll"

 QMAKE_LFLAGS=-Wl,-enable-auto-import
}

INCLUDEPATH = $${GLOBAL_INCLUDES} \
              $$PWD/libutil/src \
              $$PWD/libconexbd/src \
              $$PWD/libparsers/src \
              $$PWD/libpgmodeler/src \
              $$PWD/libobjrenderer/src \
              $$PWD/pgmodeler_ui/src

LIBS = $${GLOBAL_LIBS}

TEMPLATE = subdirs

SUBDIRS = pgmodeler_ui\
          libutil \
          libconexbd \
          libparsers \
          libpgmodeler \
          libobjrenderer

SUBDIRS = pgmodeler_ui

sources.files = models schemas lang conf
sources.path = .
INSTALLS += sources
