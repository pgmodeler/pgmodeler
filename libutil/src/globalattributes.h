/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: libutil library
# Description: Definition of GlobalAttributes namespace wich
#              stores a series of static strings constants used
#              to reference the global configuration attributes of
#              the software.
# Creation date: 14/05/2010
#
# Copyright 2006-2012 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
#ifndef GLOBAL_ATTRIBUTES_H
#define GLOBAL_ATTRIBUTES_H

#include <cstdlib>
#include <QString>
#include <QDir>

namespace GlobalAttributes {

/* Variables used to reference the pgModeler directories.
   By default, it searches the directories conf/, schemas/, lang/, plugins/ and tmp/ on
   the working dir. But these values ​​can be overwritten using the
   environment variables:

     PGMODELER_SCHEMAS_DIR
     PGMODELER_CONF_DIR
     PGMODELER_LANG_DIR
     PGMODELER_PLUGINS_DIR
     PGMODELER_TMP_DIR
*/

static const QString
  /** ticket#1 **/
  /* According to the libxml documentation , the paths used by the parser are
     in URI format (eg file://a/b/c) then, in Windows, the paths are shaped
     C:\a\b\c, this caused the error in the parser that could not find
     the DTD's. The solution to this problem is to replace the '\' by the way '/' */

  /** issue#7 **/
  /* Fix incorrect reference to environment variables.
     If the variable is not specified, pgModeler searches the required folder in the current directory "." */
  SCHEMAS_ROOT_DIR=(getenv("PGMODELER_SCHEMAS_DIR") ? QString(getenv("PGMODELER_SCHEMAS_DIR")).replace("\\","/") : QString("./schemas")),
  CONFIGURATIONS_DIR=(getenv("PGMODELER_CONF_DIR") ? QString(getenv("PGMODELER_CONF_DIR")).replace("\\","/") : QString("./conf")),
  LANGUAGES_DIR=(getenv("PGMODELER_LANG_DIR") ? QString(getenv("PGMODELER_LANG_DIR")).replace("\\","/") : QString("./lang")),
  PLUGINS_DIR=(getenv("PGMODELER_PLUGINS_DIR") ? QString(getenv("PGMODELER_PLUGINS_DIR")).replace("\\","/") : QString("./plugins")),
  TEMPORARY_DIR=(getenv("PGMODELER_TMP_DIR") ? QString(getenv("PGMODELER_TMP_DIR")).replace("\\","/") : QString("./tmp"));

static const QString
   PGMODELER_VERSION="0.4.0-alpha2",
   CRASH_HANDLER_FILE="pgmodeler%1.crash",
   STACKTRACE_FILE=".stacktrace",

   DIR_SEPARATOR="/",
   DEFAULT_CONFS_DIR="defaults", //Directory name which holds the default pgModeler configuration
   SCHEMAS_DIR="schemas", //Default name for the schemas directory
   SQL_SCHEMA_DIR="sql", //Default name for the sql schemas directory
   XML_SCHEMA_DIR="xml", //Default name for the xml schemas directory
   COMMON_SCHEMA_DIR="common", //Default name for the commom schemas directory
   SCHEMA_EXT=".sch", //Default extension for schema files
   OBJECT_DTD_DIR="dtd", //Default directory for dtd files
   OBJECT_DTD_EXT=".dtd", //Default extension for dtd files
   ROOT_DTD="dbmodel", //Root DTD of model xml files
   CONFIGURATION_EXT=".conf", //Default extension for configuration files
   HIGHLIGHT_FILE_SUF="-highlight", //Suffix of language highlight configuration files

   CODE_HIGHLIGHT_CONF="source-code-highlight", //Default name for the language highlight dtd
   OBJECTS_STYLE_CONF="objects-style", //Default name for the object style configuration file
   GENERAL_CONF="pgmodeler", //Default name for the general pgModeler configuration
   CONNECTIONS_CONF="connections", //Default name for the DBMS connection configuration file

   SQL_HIGHLIGHT_CONF="sql-highlight", //Configuration file for SQL language highlight
   XML_HIGHLIGHT_CONF="xml-highlight", //Configuration file for XML language highlight

   EXAMPLE_MODEL="example.pgmodel"; //Default name for the sample model loaded on appearence configuration form
}

#endif
