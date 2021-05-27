include(../../pgmodeler.pri)

TEMPLATE = lib
TARGET = pgmcanvas
windows: DESTDIR = $$PWD

HEADERS +=  src/baseobjectview.h \
	src/layeritem.h \
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
	src/layeritem.cpp \
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

unix|windows: LIBS += $$PGMCORE_LIB \
		      $$PGMPARSERS_LIB \
		      $$PGMUTILS_LIB

INCLUDEPATH += $$PGMCORE_INC \
	       $$PGMPARSERS_INC \
	       $$PGMUTILS_INC

DEPENDPATH += $$PGMCORE_ROOT \
	      $$PGMPARSERS_ROOT \
	      $$PGMUTILS_ROOT

# Deployment settings
target.path = $$PRIVATELIBDIR
INSTALLS = target
