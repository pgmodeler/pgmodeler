/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
	PgModelerAppName,
	PgModelerURI,
	PgModelerVersion,
	PgModelerBuildNumber,
	PgModelerSite,
	PgModelerSupport,
	PgModelerSourceURL,
	PgModelerDownloadURL,
	PgModelerDonateURL,
	PgModelerUpdateCheckURL,

	BugReportEmail,
	BugReportFile,
	StacktraceFile,

	DirSeparator,
	DefaultConfsDir,  //! \brief Directory name which holds the default pgModeler configuration
	ConfsBackupsDir,  //! \brief Directory name which holds the pgModeler configuration backups
	SchemasDir,        //! \brief Default name for the schemas directory
	SQLSchemaDir,     //! \brief Default name for the sql schemas directory
	XMLSchemaDir,     //! \brief Default name for the xml schemas directory
	AlterSchemaDir,   //! \brief Default name for the alter schemas directory
	SchemaExt,         //! \brief Default extension for schema files
	ObjectDTDDir,     //! \brief Default directory for dtd files
	ObjectDTDExt,     //! \brief Default extension for dtd files
	RootDTD,           //! \brief Root DTD of model xml files
	MetadataDTD,				//! \brief Root DTD of objects metadata xml files
	ConfigurationExt,  //! \brief Default extension for configuration files
	HighlightFileSuffix, //! \brief Suffix of language highlight configuration files

	CodeHighlightConf,  //! \brief Default name for the language highlight dtd
	ObjectsStyleConf,   //! \brief Default name for the object style configuration file
	GeneralConf,         //! \brief Default name for the general pgModeler configuration
	ConnectionsConf,     //! \brief Default name for the DBMS connection configuration file
	RelationshipsConf,   //! \brief Default name for the relationships configuration file
	SnippetsConf,        //! \brief Default name for the code snippets configuration file

	SQLHighlightConf, //! \brief Configuration file for SQL language highlight
	XMLHighlightConf, //! \brief Configuration file for XML language highlight
	PatternHighlightConf, //! \brief Configuration file for name patterns highlight (relationship editing form)
	SQLHistoryConf,		//! \brief Default name for the SQL commands history configuration file

	ExampleModel, //! \brief Default name for the sample model loaded on appearence configuration form
	UiStyleConf, //! \brief Configuration file ui style

	/*! \brief Fusion is the default widget style for pgModeler. User can change this by calling
  the executable using -style option. This same style is applied to crash handler. */
	DefaultQtStyle,
	UiStyleOption;


	/*! \brief Environment variables used to reference the pgModeler directories.

	 PGMODELER_SCHEMAS_DIR   --> "schemas" folder  (SQL/XML generation schema files)
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
	SchemasRootDir,
	LanguagesDir,
	PluginsDir,
	TemporaryDir,
	SamplesDir,
	TmplConfigurationDir,
	ConfigurationsDir,
	SQLHighlightConfPath,
	XMLHighlightConfPath,
	PgModelerCHandlerPath,
	PgModelerCLIPath,
	PgModelerAppPath;

#ifdef DEMO_VERSION
	//Maximum object creation counter for demo version
	extern const unsigned MaxObjectCount;
#endif

	/*! \brief Returns the current value for an environment variable. If the current value is a path and the same does not
	  exists then the function will return 'default_value' if it exists. Finally, if both current value and default
	  values does not exists the the fallback value is returned even if it not exists in the filesystem */
	extern QString getPathFromEnv(const QString &varname, const QString &default_val, const QString &fallback_val=QString());
}

#endif
