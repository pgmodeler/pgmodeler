/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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
\brief Definition of GlobalAttributes class which stores a series of static strings constants used
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

class GlobalAttributes {
	private:
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

		static QString
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

		/*! \brief Returns the current value for an environment variable. If the current value is a path and the same does not
			exists then the function will return 'default_value' if it exists. Finally, if both current value and default
			values does not exists the the fallback value is returned even if it not exists in the filesystem */
		static QString getPathFromEnv(const QString &varname, const QString &default_val, const QString &fallback_val=QString());

		//! \brief Sets the path in which the application should search for its internal folders (schemas, lang, conf, etc)
		static void setSearchPath(const QString &search_path);

	public:
		static const QString
		PgModelerAppName,
		PgModelerURI,
		PgModelerReverseURI,
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
		CatalogSchemasDir,//! \brief Default name for the catalog schemas directory
		DataDictSchemaDir,//! \brief Default name for the data dictionary schemas directory
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
		DiffPresetsConf,     //! \brief Default name for the diff presets configuration file

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

		#ifdef DEMO_VERSION
			//Maximum object creation counter for demo version
			static constexpr unsigned MaxObjectCount = 15;
		#endif

		//! \brief Returns the path to the "schemas" folder
		static QString getSchemasRootDir();

		/*! \brief Returns the path to a schema file under "schemas" folder.
		 * Since this method only operates over schemas folder there's no need to
		 * provide a file with extension because the method already appends the extension .sch
		 * automatically. */
		static QString getSchemaFilePath(const QString &subfolder, const QString &file);

		//! \brief Returns the path to the "tmp" folder in user's local storage
		static QString getTemporaryDir();

		//! \brief Returns the path to a temp file under "tmp" folder at user's local storage
		static QString getTemporaryFilePath(const QString &file);

		//! \brief Returns the path to the template "conf" folder in pgModeler's installation
		static QString getTmplConfigurationDir();

		/*! \brief Returns the path to the template file at template "conf" folder in pgModeler's installation
		 * This method will not append any extension to the file since this folder has several kinds of
		 * files inside it, so, the user must inform the file with its extension */
		static QString getTmplConfigurationFilePath(const QString &subfolder, const QString &file);

		//! \brief Returns the path to the "conf" folder in user's local storage
		static QString getConfigurationsDir();

		/*! \brief Returns the path to a config file under "conf" folder at user's local storage.
		 * There's no need to specify the extension (.conf) since the method will automatically append it. */
		static QString getConfigurationFilePath(const QString &file);

		//! \brief Returns the path to the "samples" folder in pgModeler's installation
		static QString getSamplesDir();

		//! \brief Returns the path to the "lang" folder in pgModeler's installation
		static QString getLanguagesDir();

		//! \brief Returns the path to the "plugins" folder in pgModeler's installation
		static QString getPluginsDir();

		//! \brief Returns the path to the "sql-highlight.conf" file in user's local storage
		static QString getSQLHighlightConfPath();

		//! \brief Returns the path to the "xml-highlight.conf" file in user's local storage
		static QString getXMLHighlightConfPath();

		//! \brief Returns the path to the "pgmodeler-ch" (crash handler) executable
		static QString getPgModelerCHandlerPath();

		//! \brief Returns the path to the "pgmodeler-cli" (command line interface) executable
		static QString getPgModelerCLIPath();

		//! \brief Returns the path to the "pgmodeler" (main application) executable
		static QString getPgModelerAppPath();

		friend class Application;
};

#endif
