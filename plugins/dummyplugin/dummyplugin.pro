include(../pgmodeler.pro)
include(../libutil/libutil.pro)
include(../libconexbd/libconexbd.pro)
include(../libparsers/libparsers.pro)
include(../libpgmodeler/libpgmodeler.pro)
include(../libobjrenderer/libobjrenderer.pro)

CONFIG += qt warn_on uitools uic4
QT = core gui qt3support
TEMPLATE = lib
TARGET = build/plugins/dummyplugin

TRANSLATIONS = lang/dummyplugin.en_US.ts

DEPENDPATH = ". res src ui moc obj"
MOC_DIR = moc
OBJECTS_DIR = obj
UI_DIR = src
DESTDIR = ../build/plugin
CODECFORTR = UTF-8
RESOURCES += res/resources.qrc

HEADERS += \
    src/dummyplugin.h

SOURCES += \
    src/dummyplugin.cpp
