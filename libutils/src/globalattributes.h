/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2015 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

/**
\ingroup libutils
\namespace GlobalAttributes
\brief Definition of GlobalAttributes namespace wich stores a series of static strings constants used
 to reference the global configuration attributes of the software.
\note <strong>Creation date:</strong> 14/05/2010
*/

#ifndef GLOBAL_ATTRIBUTES_H
#define GLOBAL_ATTRIBUTES_H

/* Including QByteArray due to 'QByteArray has no toStdString()'
   error on Qt 5.4 (Windows only) */
#include <QByteArray>
#include <cstdlib>
#include <QString>
#include <QDir>
#include <QDate>
#include <QStandardPaths>

namespace GlobalAttributes {
	static const QString
  PGMODELER_APP_NAME=QString("pgmodeler"),
  PGMODELER_URI=QString("pgmodeler.com.br"),
  PGMODELER_VERSION=QString("0.8.0-beta2"),
  PGMODELER_VER_CODENAME=QString("Faithful Elephant"),
  PGMODELER_BUILD_NUMBER=QDate::fromString(QString(__DATE__).simplified(), "MMM d yyyy").toString("yyyyMMdd"),
  PGMODELER_SITE=QString("http://www.pgmodeler.com.br"),
  PGMODELER_WIKI=QString("%1/wiki").arg(PGMODELER_SITE),
  PGMODELER_SRC_URL=QString("https://github.com/pgmodeler/pgmodeler/releases"),
  PGMODELER_BIN_URL=QString("%1/purchase.php").arg(PGMODELER_SITE),
  PGMODELER_UPD_CHECK_URL=QString("%1/checkupdate.php?current_ver=").arg(PGMODELER_SITE),

  BUG_REPORT_EMAIL=QString("bug@pgmodeler.com.br"),
  BUG_REPORT_FILE=QString("pgmodeler%1.bug"),
  STACKTRACE_FILE=QString(".stacktrace"),
  MACOS_STARTUP_SCRIPT=QString("startapp"),

  DIR_SEPARATOR=QString("/"),
  DEFAULT_CONFS_DIR=QString("defaults"), //! \brief Directory name which holds the default pgModeler configuration
  SCHEMAS_DIR=QString("schemas"), //! \brief Default name for the schemas directory
  SQL_SCHEMA_DIR=QString("sql"), //! \brief Default name for the sql schemas directory
  XML_SCHEMA_DIR=QString("xml"), //! \brief Default name for the xml schemas directory
  ALTER_SCHEMA_DIR=QString("alter"), //! \brief Default name for the alter schemas directory
  SCHEMA_EXT=QString(".sch"), //! \brief Default extension for schema files
  OBJECT_DTD_DIR=QString("dtd"), //! \brief Default directory for dtd files
  OBJECT_DTD_EXT=QString(".dtd"), //! \brief Default extension for dtd files
  ROOT_DTD=QString("dbmodel"), //! \brief Root DTD of model xml files
  CONFIGURATION_EXT=QString(".conf"), //! \brief Default extension for configuration files
  HIGHLIGHT_FILE_SUF=QString("-highlight"), //! \brief Suffix of language highlight configuration files

  CODE_HIGHLIGHT_CONF=QString("source-code-highlight"), //! \brief Default name for the language highlight dtd
  OBJECTS_STYLE_CONF=QString("objects-style"), //! \brief Default name for the object style configuration file
  GENERAL_CONF=QString("pgmodeler"), //! \brief Default name for the general pgModeler configuration
  CONNECTIONS_CONF=QString("connections"), //! \brief Default name for the DBMS connection configuration file
  RELATIONSHIPS_CONF=QString("relationships"),//! \brief Default name for the relationships configuration file
  SNIPPETS_CONF=QString("snippets"),//! \brief Default name for the code snippets configuration file

  SQL_HIGHLIGHT_CONF=QString("sql-highlight"), //! \brief Configuration file for SQL language highlight
  XML_HIGHLIGHT_CONF=QString("xml-highlight"), //! \brief Configuration file for XML language highlight
  PATTERN_HIGHLIGHT_CONF=QString("pattern-highlight"), //! \brief Configuration file for name patterns highlight (relationship editing form)

  EXAMPLE_MODEL=QString("example.dbm"), //! \brief Default name for the sample model loaded on appearence configuration form
  UI_STYLE_CONF=QString("ui-style"), //! \brief Configuration file ui style

	/*! \brief Fusion is the default widget style for pgModeler. User can change this by calling
	the executable using -style option. This same style is applied to crash handler. */
  DEFAULT_QT_STYLE=QString("Fusion"),
  UI_STYLE_OPT=QString("-style");


	/*! \brief Variables used to reference the pgModeler directories.
	 By default, it searches the directories conf/, schemas/, lang/, plugins/, tmp/ and samples/ on
	 the working dir. But these values ​​can be overwritten using the
	 environment variables:

		 PGMODELER_SCHEMAS_DIR
		 PGMODELER_CONF_DIR
		 PGMODELER_LANG_DIR
		 PGMODELER_PLUGINS_DIR
		 PGMODELER_TMP_DIR
     PGMODELER_SAMPLES_DIR

   Additional var are used to specify where to find crash handler and command line interface
   application.

     PGMODELER_CHANDLER_PATH
     PGMODELER_CLI_PATH */

	static const QString
	/*! \brief According to the libxml documentation , the paths used by the parser are
		 in URI format (eg file://a/b/c) then, in Windows, the paths are shaped
		 C:\a\b\c, this caused the error in the parser that could not find
		 the DTD's. The solution to this problem is to replace the '\' by the way '/' */

	/*! \brief If the variable is not specified, pgModeler searches the required folder in the current directory "." */
  SCHEMAS_ROOT_DIR=getenv("PGMODELER_SCHEMAS_DIR") ? QString(getenv("PGMODELER_SCHEMAS_DIR")).replace('\\','/') : QString(SCHEMASDIR),
  LANGUAGES_DIR=getenv("PGMODELER_LANG_DIR") ? QString(getenv("PGMODELER_LANG_DIR")).replace('\\','/') : QString(LANGDIR),
  PLUGINS_DIR=getenv("PGMODELER_PLUGINS_DIR") ? QString(getenv("PGMODELER_PLUGINS_DIR")).replace('\\','/') : QString(PLUGINSDIR),
  TEMPORARY_DIR=getenv("PGMODELER_TMP_DIR") ? QString(getenv("PGMODELER_TMP_DIR")).replace('\\','/') : QString(TEMPDIR),
  SAMPLES_DIR=getenv("PGMODELER_SAMPLES_DIR") ? QString(getenv("PGMODELER_SAMPLES_DIR")).replace('\\','/') : QString(SAMPLESDIR),

  #if defined(Q_OS_MAC)
    CONFIGURATIONS_DIR=getenv("PGMODELER_CONF_DIR") ?
                       QString(getenv("PGMODELER_CONF_DIR")).replace('\\','/') :
                       QString(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/br.com.pgmodeler")),
  #elif defined(Q_OS_LINUX)
    CONFIGURATIONS_DIR=getenv("PGMODELER_CONF_DIR") ?
                       QString(getenv("PGMODELER_CONF_DIR")).replace('\\','/') :
                       QString(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QString("/pgmodeler")),
  #else
    CONFIGURATIONS_DIR=getenv("PGMODELER_CONF_DIR") ?
                       QString(getenv("PGMODELER_CONF_DIR")).replace('\\','/') :
                       QString(QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QString("/pgmodeler")),
  #endif

  SQL_HIGHLIGHT_CONF_PATH=CONFIGURATIONS_DIR + DIR_SEPARATOR + SQL_HIGHLIGHT_CONF + CONFIGURATION_EXT,
  XML_HIGHLIGHT_CONF_PATH=CONFIGURATIONS_DIR + DIR_SEPARATOR + XML_HIGHLIGHT_CONF + CONFIGURATION_EXT,


  /*! \brief Crash handler and CLI executables path configuration, the user can use the below envvar to set a
       different location for pgmodeler-ch as well pgmodeler-cli */
  #if defined(Q_OS_UNIX)
    #if defined(Q_OS_MAC)
      //For MacOSX the crash handler path is fixed (inside bundle)
      CRASH_HANDLER_PATH=MACOS_STARTUP_SCRIPT + QString(" pgmodeler-ch"),
      PGMODELER_CLI_PATH=MACOS_STARTUP_SCRIPT;// + QString(" pgmodeler-cli");
    #else
      CRASH_HANDLER_PATH=getenv("PGMODELER_CHANDLER_PATH") ?
                         QString(getenv("PGMODELER_CHANDLER_PATH")) :
                         QString(PRIVATEBINDIR) + QString("/pgmodeler-ch"),

      PGMODELER_CLI_PATH=getenv("PGMODELER_CLI_PATH") ?
                           QString(getenv("PGMODELER_CLI_PATH")) :
                           QString(BINDIR) + QString("/pgmodeler-cli");
    #endif
  #else
    CRASH_HANDLER_PATH=getenv("PGMODELER_CHANDLER_PATH") ?
                       QString(getenv("PGMODELER_CHANDLER_PATH")) :
                       QString(PRIVATEBINDIR) + QString("\\pgmodeler-ch.exe"),

    PGMODELER_CLI_PATH=getenv("PGMODELER_CLI_PATH") ?
                       QString(getenv("PGMODELER_CLI_PATH")) :
                       QString(PRIVATEBINDIR) + QString("\\pgmodeler-cli.exe");
  #endif

	#ifdef DEMO_VERSION
	 //Maximum object creation counter for demo version
   static const unsigned MAX_OBJECT_COUNT=8;
	#endif  
}

#endif
