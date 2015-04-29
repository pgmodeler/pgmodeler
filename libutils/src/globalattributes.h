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
  extern const QString
  PGMODELER_APP_NAME,
  PGMODELER_URI,
  PGMODELER_VERSION,
  PGMODELER_VER_CODENAME,
  PGMODELER_BUILD_NUMBER,
  PGMODELER_SITE,
  PGMODELER_WIKI,
  PGMODELER_SRC_URL,
  PGMODELER_PURCHASE_URL,
  PGMODELER_RECOVER_URL,
  PGMODELER_DONATE_URL,
  PGMODELER_UPD_CHECK_URL,

  BUG_REPORT_EMAIL,
  BUG_REPORT_FILE,
  STACKTRACE_FILE,

  DIR_SEPARATOR,
  DEFAULT_CONFS_DIR,  //! \brief Directory name which holds the default pgModeler configuration
  SCHEMAS_DIR,        //! \brief Default name for the schemas directory
  SQL_SCHEMA_DIR,     //! \brief Default name for the sql schemas directory
  XML_SCHEMA_DIR,     //! \brief Default name for the xml schemas directory
  ALTER_SCHEMA_DIR,   //! \brief Default name for the alter schemas directory
  SCHEMA_EXT,         //! \brief Default extension for schema files
  OBJECT_DTD_DIR,     //! \brief Default directory for dtd files
  OBJECT_DTD_EXT,     //! \brief Default extension for dtd files
  ROOT_DTD,           //! \brief Root DTD of model xml files
  CONFIGURATION_EXT,  //! \brief Default extension for configuration files
  HIGHLIGHT_FILE_SUF, //! \brief Suffix of language highlight configuration files

  CODE_HIGHLIGHT_CONF,  //! \brief Default name for the language highlight dtd
  OBJECTS_STYLE_CONF,   //! \brief Default name for the object style configuration file
  GENERAL_CONF,         //! \brief Default name for the general pgModeler configuration
  CONNECTIONS_CONF,     //! \brief Default name for the DBMS connection configuration file
  RELATIONSHIPS_CONF,   //! \brief Default name for the relationships configuration file
  SNIPPETS_CONF,        //! \brief Default name for the code snippets configuration file

  SQL_HIGHLIGHT_CONF, //! \brief Configuration file for SQL language highlight
  XML_HIGHLIGHT_CONF, //! \brief Configuration file for XML language highlight
  PATTERN_HIGHLIGHT_CONF, //! \brief Configuration file for name patterns highlight (relationship editing form)

  EXAMPLE_MODEL, //! \brief Default name for the sample model loaded on appearence configuration form
  UI_STYLE_CONF, //! \brief Configuration file ui style

  /*! \brief Fusion is the default widget style for pgModeler. User can change this by calling
  the executable using -style option. This same style is applied to crash handler. */
  DEFAULT_QT_STYLE,
  UI_STYLE_OPT;


  /*! \brief Variables used to reference the pgModeler directories.

     PGMODELER_SCHEMAS_DIR   --> "schema" folder  (SQL/XML generation schema files)
     PGMODELER_CONF_DIR      --> "conf" folder    (user's own settings for pgModeler)
     PGMODELER_TMPL_CONF_DIR --> "conf" folder    (used as template settings and copied to user's settings)
     PGMODELER_LANG_DIR      --> "lang" folder    (ui translations)
     PGMODELER_PLUGINS_DIR   --> "plugins" folder (where plugins are installed)
     PGMODELER_TMP_DIR       --> "tmp" folder     (where temporary work are saved)
     PGMODELER_SAMPLES_DIR   --> "samples" folder (contains sample dbm files)

   Additional vars are used to specify where to find crash handler, command line interface
   and main application.

     PGMODELER_CHANDLER_PATH --> Full path to pgmodeler-ch executable
     PGMODELER_CLI_PATH      --> Full path to pgmodeler-cli executable
     PGMDOELER_APP_PATH      --> Full path to pgmodeler executable */

  extern const QString
  SCHEMAS_ROOT_DIR,
  LANGUAGES_DIR,
  PLUGINS_DIR,
  TEMPORARY_DIR,
  SAMPLES_DIR,
  TMPL_CONFIGURATIONS_DIR,
  CONFIGURATIONS_DIR,
  SQL_HIGHLIGHT_CONF_PATH,
  XML_HIGHLIGHT_CONF_PATH,
  PGMODELER_CHANDLER_PATH,
  PGMODELER_CLI_PATH,
  PGMODELER_APP_PATH;

  #ifdef DEMO_VERSION
   //Maximum object creation counter for demo version
   extern const unsigned MAX_OBJECT_COUNT;
  #endif
}

#endif
