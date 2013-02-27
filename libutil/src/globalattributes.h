/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
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
\ingroup libutil
\namespace GlobalAttributes
\brief Definition of GlobalAttributes namespace wich stores a series of static strings constants used
 to reference the global configuration attributes of the software.
\note <strong>Creation date:</strong> 14/05/2010
*/

#ifndef GLOBAL_ATTRIBUTES_H
#define GLOBAL_ATTRIBUTES_H

#include <cstdlib>
#include <QString>
#include <QDir>

namespace GlobalAttributes {

	/*! \brief Variables used to reference the pgModeler directories.
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
	/*! \brief According to the libxml documentation , the paths used by the parser are
		 in URI format (eg file://a/b/c) then, in Windows, the paths are shaped
		 C:\a\b\c, this caused the error in the parser that could not find
		 the DTD's. The solution to this problem is to replace the '\' by the way '/' */

	/*! \brief If the variable is not specified, pgModeler searches the required folder in the current directory "." */
	SCHEMAS_ROOT_DIR=(getenv("PGMODELER_SCHEMAS_DIR") ? QString(getenv("PGMODELER_SCHEMAS_DIR")).replace("\\","/") : QString("./schemas")),
	CONFIGURATIONS_DIR=(getenv("PGMODELER_CONF_DIR") ? QString(getenv("PGMODELER_CONF_DIR")).replace("\\","/") : QString("./conf")),
	LANGUAGES_DIR=(getenv("PGMODELER_LANG_DIR") ? QString(getenv("PGMODELER_LANG_DIR")).replace("\\","/") : QString("./lang")),
	PLUGINS_DIR=(getenv("PGMODELER_PLUGINS_DIR") ? QString(getenv("PGMODELER_PLUGINS_DIR")).replace("\\","/") : QString("./plugins")),
	TEMPORARY_DIR=(getenv("PGMODELER_TMP_DIR") ? QString(getenv("PGMODELER_TMP_DIR")).replace("\\","/") : QString("./tmp"));

	static const QString
	PGMODELER_VERSION="0.4.0",
	CRASH_HANDLER_FILE="pgmodeler%1.crash",
	STACKTRACE_FILE=".stacktrace",

	DIR_SEPARATOR="/",
	DEFAULT_CONFS_DIR="defaults", //! \brief Directory name which holds the default pgModeler configuration
	SCHEMAS_DIR="schemas", //! \brief Default name for the schemas directory
	SQL_SCHEMA_DIR="sql", //! \brief Default name for the sql schemas directory
	XML_SCHEMA_DIR="xml", //! \brief Default name for the xml schemas directory
	COMMON_SCHEMA_DIR="common", //! \brief Default name for the commom schemas directory
	SCHEMA_EXT=".sch", //! \brief Default extension for schema files
	OBJECT_DTD_DIR="dtd", //! \brief Default directory for dtd files
	OBJECT_DTD_EXT=".dtd", //! \brief Default extension for dtd files
	ROOT_DTD="dbmodel", //! \brief Root DTD of model xml files
	CONFIGURATION_EXT=".conf", //! \brief Default extension for configuration files
	HIGHLIGHT_FILE_SUF="-highlight", //! \brief Suffix of language highlight configuration files

	CODE_HIGHLIGHT_CONF="source-code-highlight", //! \brief Default name for the language highlight dtd
	OBJECTS_STYLE_CONF="objects-style", //! \brief Default name for the object style configuration file
	GENERAL_CONF="pgmodeler", //! \brief Default name for the general pgModeler configuration
	CONNECTIONS_CONF="connections", //! \brief Default name for the DBMS connection configuration file

	SQL_HIGHLIGHT_CONF="sql-highlight", //! \brief Configuration file for SQL language highlight
	XML_HIGHLIGHT_CONF="xml-highlight", //! \brief Configuration file for XML language highlight

	EXAMPLE_MODEL="example.dbm"; //! \brief Default name for the sample model loaded on appearence configuration form
}

#endif
