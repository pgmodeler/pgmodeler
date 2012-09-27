include(../pgmodeler.pro)

CONFIG += qt warn_on uitools uic4
QT = core gui qt3support
TEMPLATE = app
TARGET = pgmodeler

DEPENDPATH = ". moc obj"
OBJECTS_DIR = obj
DESTDIR = ../build

unix {
 QMAKE_POST_LINK+= "cp -r ../schemas/ $$DESTDIR; \
                    cp -r ../conf/ $$DESTDIR; \
                    cp -r ../lang/ $$DESTDIR; \
                    cp -r ../samples $$DESTDIR; \
                    cp -r ../*.md $$DESTDIR; \
                    cp -r ../LICENSE $$DESTDIR; \
                    cp -r ../start-pgmodeler.sh $$DESTDIR"

 QMAKE_DISTCLEAN+= "-r $$DESTDIR/*"
}

windows {
 DEST=..\\build
 QMAKE_POST_LINK+="xcopy ..\\schemas $$DEST\\schemas /S /I /Y /Q & \
                   xcopy ..\\conf $$DEST\\conf /S /I /Y /Q & \
                   xcopy ..\\lang $$DEST\\lang /S /I /Y /Q & \
                   xcopy ..\\samples $$DEST\\samples /S /I /Y /Q & \
                   copy ..\\*.md $$DEST /Y & \
                   copy ..\\LICENSE $$DEST /Y & \
                   copy ..\\start-pgmodeler.bat $$DEST /Y "

 QMAKE_DISTCLEAN+= " $$DEST\\* /S /Q & \
                     rmdir $$DEST\\schemas /S /Q & \
                     rmdir $$DEST\\conf /S /Q & \
                     rmdir $$DEST\\lang /S /Q & \
                     rmdir $$DEST\\samples /S /Q & \
                     rmdir $$DEST\\plugins /S /Q  & \
                     rmdir $$DEST\\lib /S /Q"
}


LIBS = $$DESTDIR/lib/$$LIBUTIL \
       $$DESTDIR/lib/$$LIBPARSERS \
       $$DESTDIR/lib/$$LIBCONEXBD \
       $$DESTDIR/lib/$$LIBOBJRENDERER \
       $$DESTDIR/lib/$$LIBPGMODELER \
       $$DESTDIR/lib/$$LIBPGMODELERUI

HEADERS += $$PWD/src/aplicacao.h
SOURCES += $$PWD/src/main.cpp
