# libobjrenderer.pro (reviewed version)
#
# Refactored by: Lisandro Damián Nicanor Pérez Meyer <perezmeyer@gmail.com>
# Refactored code: https://github.com/perezmeyer/pgmodeler/tree/shared_libs
# Reviewed by: Raphal Araújo e Silva <raphael@pgmodeler.com.br>
#
# NOTE: Reviewed code is not a direct merge from refactored version but based upon the
# refactored code, containing almost all changes done by the refactoring author.

include(../pgmodeler.pri)

TEMPLATE = lib
TARGET = objrenderer
windows: DESTDIR = $$PWD

HEADERS +=  src/baseobjectview.h \
	    src/textboxview.h \
	    src/tableview.h \
	    src/graphicalview.h \
	    src/relationshipview.h \
	    src/tabletitleview.h \
	    src/tableobjectview.h \
	    src/basetableview.h \
	    src/objectsscene.h \
            src/schemaview.h \
            src/roundedrectitem.h \
            src/styledtextboxview.h \
	    src/beziercurveitem.h \
	    src/textpolygonitem.h \
    src/attributestoggleritem.h

SOURCES +=  src/baseobjectview.cpp \
	    src/textboxview.cpp \
	    src/tableview.cpp \
	    src/graphicalview.cpp \
	    src/relationshipview.cpp \
	    src/tabletitleview.cpp \
	    src/tableobjectview.cpp \
	    src/basetableview.cpp \
	    src/objectsscene.cpp \
	    src/schemaview.cpp \
            src/roundedrectitem.cpp \
            src/styledtextboxview.cpp \
	    src/beziercurveitem.cpp \
	    src/textpolygonitem.cpp \
    src/attributestoggleritem.cpp

unix|windows: LIBS += -L$$OUT_PWD/../libpgmodeler/ -lpgmodeler \
                    -L$$OUT_PWD/../libparsers/ -lparsers \
                    -L$$OUT_PWD/../libutils/ -lutils

INCLUDEPATH += $$PWD/../libpgmodeler/src \
               $$PWD/../libparsers/src \
	       $$PWD/../libutils/src

DEPENDPATH += $$PWD/../libpgmodeler \
              $$PWD/../libparsers \
	      $$PWD/../libutils

# Deployment settings
target.path = $$PRIVATELIBDIR
INSTALLS = target
