include(../pgmodeler.pri)

TEMPLATE = lib
TARGET = objrenderer

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
            src/styledtextboxview.h

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
            src/styledtextboxview.cpp

unix|win32: LIBS += -L$$OUT_PWD/../libpgmodeler/ -lpgmodeler \
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
