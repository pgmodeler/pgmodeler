/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2024 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "utilsglobal.h"
#include "attribsmap.h"
#include <QString>
#include <QStandardPaths>

class __libutils GlobalAttributes {
	private:
		/*! \brief Environment variables used to reference the pgModeler directories.
		 * These environment variables can be referenced in code using Env*Path constants.

		 PGMODELER_SCHEMAS_PATH   --> full path to the "schemas" folder  (SQL/XML generation schema files)
		 PGMODELER_CONF_PATH      --> full path to the "conf" folder    (user's own settings for pgModeler)
		 PGMODELER_TMPL_CONF_PATH --> full path to the "conf" folder    (used as template settings and copied to user's settings)
		 PGMODELER_LANG_PATH      --> full path to the "lang" folder    (ui translations)
		 PGMODELER_PLUGINS_PATH   --> full path to the "plugins" folder (where plugins are installed)
		 PGMODELER_TMP_PATH       --> full path to the "tmp" folder     (where temporary work are saved)
		 PGMODELER_SAMPLES_PATH   --> full path to the "samples" folder (contains sample dbm files)

		 Additional vars are used to specify where to find crash handler, command line interface
		 and main application.

		 PGMODELER_CH_PATH  --> Full path to pgmodeler-ch executable
		 PGMODELER_CLI_PATH --> Full path to pgmodeler-cli executable
		 PGMODELER_SE_PATH  --> Full path to pgmodeler-se executable
		 PGMODELER_PATH     --> Full path to pgmodeler executable */

		inline static QString
		SchemasRootPath,
		LanguagesPath,
		PluginsPath,
		TemporaryPath,
		SamplesPath,
		TmplConfigurationPath,
		ConfigurationsPath,
		SQLHighlightConfPath,
		XMLHighlightConfPath,
		SchHighlightConfPath,
		PatternHighlightConfPath,
		PgModelerCHandlerPath,
		PgModelerCLIPath,
		PgModelerAppPath,
		PgModelerSchemaEditorPath,
		PgModelerBaseVersion;

		//! \brief Stores the custom paths retrieved either from pgmpaths.conf or from environment variables
		inline static attribs_map CustomPaths;

		/*! \brief Returns the current value for an environment variable that points to one of the assets/executables.
		 *  These variables can be defined either in the file pgmpaths.conf (key=value file) or from system's environment variables.
		 *
		 * If the current value points to an inexstent path then the function will return 'default_value' if it exists.
		 * Finally, if both current value and default values don't exist then the fallback value is returned even if it not
		 * exists in the filesystem.
		 *
		 * The precedence of value retriaval is the following:
		 *	pgmpaths.conf -> system's env vars -> default_value -> fallback_val */
		static QString getPathFromEnv(const QString &varname, const QString &default_val, const QString &fallback_val="");

		//! \brief Sets the config files paths variables that doesn't depend on a search path.
		static void setConfigFilesPaths();

		/*! \brief Configures the assets/executables paths loading from pgmpaths.conf or from environment variables.
		 * The search_path is from where the pgmpaths.conf must be loaded. */
		static void setCustomPaths(const QString &search_path);

		 //! \brief Sets the path in which the application should search for its internal folders (schemas, lang, conf, etc)
		static void setSearchPath(const QString &search_path);

		/*! \brief Returns the path to a file that is under a root path. Generally the root path is the full path to a folder that
		 * stores assets related to pgModeler (config files, schema files, etc) in its installation root.
		 *
		 * The parameter file_ext is the file extension that is appended to the requested file.
		 * The parameter root_path is the base path where the file name (and eventually subfolders) will be appended
		 * The parameter subfolder is a base subfolder where the file is located
		 * The paramter pack ...pth_elems is the last part of the complete file path. By using this, one can form a path with a
		 * variable number of subfolders. The last element of the parameter pack will be ALWAYS the file that is needed to be retrieved.
		 *
		 * Example:
		 *
		 * > Input: file_ext = .sch, root_path = /usr/local/pgmodeler/conf, subfolder = schemas, pth_elems = [ sql, table ]
		 * > Ouput: /usr/local/pgmodeler/conf/schemas/table.sch
		 *
		 * > Input: file_ext = [empty], root_path = ~/.config/pgmodeler, subfolder = tmp, pth_elems = [ tmpModel.dbm ]
		 * > Ouput: ~/.config/pgmodeler/tmp/tmpModel.dbm
		 *
		 * > Input: file_ext = [empty], root_path = ~/.config/pgmodeler, subfolder = [empty], pth_elems = [ empty ]
		 * > Ouput: ~/.config/pgmodeler
		 * */
		template<typename ...args>
		static QString getFilePath(const QString &file_ext, const QString &root_path, args... pth_elems)
		{
			QStringList path_elems = { pth_elems... };
			QString file, path = root_path;

			path_elems.removeAll("");

			if(path_elems.isEmpty())
				return path;

			// The last element of the parameter pack is the file to be retrieved
			file = path_elems.last();
			path_elems.removeLast();

			for(auto &folder : path_elems)
				path += DirSeparator + folder;

			return path + DirSeparator + file + (file_ext.isEmpty() ? "" : file_ext);
		}

	public:
		inline static const QString
		// The base version number of the tool
		PgModelerVersion {
			QString("1.2.0-alpha")

			/* Appending the snapshot build number to the version number
			 * when the external variable SNAPSHOT_BUILD is defined */
			#if defined(SNAPSHOT_BUILD)
				+ QString("_snapshot%1").arg(BUILDDATE)
			#endif
		},

		/* ATTENTION: The attributes below indicates the default names of configuration
		 * directories in user's local storage in the current major version as well as
		 * in the previous major version.
		 *
		 * This is used to automatically copy configuration files config from a previous
		 * version to the new one. More details in Application::createUserConfiguration()
		 * !!! ALWAYS UPDATE THESE VARIABLES WHEN UPGRADING THE MAJOR VERSION !!! */
		PgModelerAppName {"pgmodeler-1.2"},

		/* In versions 1.0.x and 1.1.x there was no distinction in the configuration directory.
		 * So, until we upgrade to 1.3, we have to use the pgmodeler-1.0 as the previous
		 * version configuration directory */
		PgModelerOldAppName {"pgmodeler-1.0"},

		PgModelerURI {"pgmodeler.io"},
		PgModelerBuildNumber { QString("%1.%2").arg(BUILDDATE).arg(BUILDNUM) },
		PgModelerSite {"https://pgmodeler.io" },
		PgModelerSupport {"https://pgmodeler.io/support/docs" },
		PgModelerSourceURL {"https://github.com/pgmodeler/pgmodeler/releases" },
		PgModelerDownloadURL { PgModelerSite + "/download" },
		PgModelerDonateURL { PgModelerSite + "/#donationForm" },
		PgModelerUpdateCheckURL { PgModelerSite + "/checkupdate?version=%1&mode=%2" },

		EnvSchemasPath {"PGMODELER_SCHEMAS_PATH"},
		EnvConfPath {"PGMODELER_CONF_PATH"},
		EnvTmplConfPath {"PGMODELER_TMPL_CONF_PATH"},
		EnvLangPath {"PGMODELER_LANG_PATH"},
		EnvPluginsPath {"PGMODELER_PLUGINS_PATH"},
		EnvTmpPath {"PGMODELER_TMP_PATH"},
		EnvSamplesPath {"PGMODELER_SAMPLES_PATH"},
		EnvPgModelerChPath {"PGMODELER_CH_PATH"},
		EnvPgModelerCliPath {"PGMODELER_CLI_PATH"},
		EnvPgModelerSePath {"PGMODELER_SE_PATH"},
		EnvPgModelerPath {"PGMODELER_PATH"},

		PgmPathsConfFile {"pgmpaths"}, //! \brief An ini-like (key=value) file that holds custom values for the pgModeler's enviroment variables
		BugReportEmail {"bug@pgmodeler.io"},
		BugReportFile {"pgmodeler%1.bug"},
		StacktraceFile {".stacktrace"},
		LastModelFile {"lastmodel"},

		DbModelExt {".dbm"},
		DbModelBkpExt {".dbk"},
		ObjMetadataExt {".omf"},
		DirSeparator {"/"},
		ResourcesDir {"res"},				 //! \brief Directory name which holds the pgModeler's plug-ins resources directory (res)
		ConfigurationsDir {"conf"},	 //! \brief Default name for the configurations directory
		DefaultConfsDir {"defaults"}, //! \brief Default name for the default configurations directory
		ConfsBackupsDir {"backups"},  //! \brief Directory name which holds the pgModeler configuration backups
		SchemasDir {"schemas"},				//! \brief Default name for the schemas directory
		SQLSchemaDir {"sql"},					//! \brief Default name for the sql schemas directory
		XMLSchemaDir {"xml"},					//! \brief Default name for the xml schemas directory
		CatalogSchemasDir {"catalog"}, //! \brief Default name for the catalog schemas directory
		DataDictSchemaDir {"datadict"}, //! \brief Default name for the data dictionary schemas root directory
		DataDictHtmlDir {"html"},				//! \brief Default name for the data dictionary schemas directory (HTML format)
		DataDictMdDir {"md"},						//! \brief Default name for the dictionary schemas directory (Markdown format)
		AlterSchemaDir {"alter"},				//! \brief Default name for the alter schemas directory
		LanguagesDir {"lang"},					//! \brief Default name for the translation files directory
		SamplesDir {"samples"},					//! \brief Default name for the samples database models directory
		PluginsDir {"plugins"},					//! \brief Default name for the plug-ins directory
		SchemaExt {".sch"},							//! \brief Default extension for schema files
		ObjectDTDDir {"dtd"},						//! \brief Default directory for dtd files
		ObjectDTDExt {".dtd"},					//! \brief Default extension for dtd files
		RootDTD {"dbmodel"},						//! \brief Root DTD of model xml files
		MetadataDTD {"metadata"},				//! \brief Root DTD of objects metadata xml files
		ConfigurationExt {".conf"},			//! \brief Default extension for configuration files
		HighlightFileSuffix {"-highlight"}, //! \brief Suffix of language highlight configuration files
		ThemesDir {"themes"},					 //! \brief Default name for the ui style directory

		CodeHighlightConf {"source-code-highlight"}, //! \brief Default name for the language highlight dtd
		AppearanceConf {"appearance"},			 //! \brief Default name for the appearance configuration file
		GeneralConf {"general"},						 //! \brief Default name for the general pgModeler configuration
		ConnectionsConf {"connections"},		 //! \brief Default name for the DBMS connection configuration file
		RelationshipsConf {"relationships"}, //! \brief Default name for the relationships configuration file
		SnippetsConf {"snippets"},					 //! \brief Default name for the code snippets configuration file
		DiffPresetsConf {"diff-presets"},		 //! \brief Default name for the diff presets configuration file

		SQLHighlightConf {"sql-highlight"}, //! \brief Configuration file for SQL language highlight
		XMLHighlightConf {"xml-highlight"}, //! \brief Configuration file for XML language highlight
		SchHighlightConf {"sch-highlight"}, //! \brief Configuration file for Schema micro-language highlight
		PatternHighlightConf {"pattern-highlight"}, //! \brief Configuration file for name patterns highlight (relationship editing form)
		SQLHistoryConf {"sql-history"}, //! \brief Default name for the SQL commands history configuration file

		ExampleModel {"example.dbm"}, //! \brief Default name for the sample model loaded on appearence configuration form
		UiStyleConf {"ui-style"},			//! \brief Configuration file ui style
		IconsMediumConf {"icons-medium"}, //! \brief Extra configuration file that defines medium icons size
		IconsSmallConf {"icons-small"},		//! \brief Extra configuration file that defines small icons size
		IconsBigConf {"icons-big"}, //! \brief Extra configuration file that defines big icons size

		FileDialogConf {"filedlg"}, //! \brief Default name for the file used to save/restore QFileDialog last geometry

		/*! \brief Fusion is the default widget style for pgModeler. User can change this by calling
		the executable using -style option. This same style is applied to crash handler. */
		DefaultQtStyle {"Fusion"},
		UiStyleOption {"-style"};

		#ifdef DEMO_VERSION
			//Maximum object creation counter for demo version
			static constexpr unsigned MaxObjectCount = 15;
		#endif

		/*! \brief Performs the initialization of the global attributes by setting the
		 * application's search path (and all assets/executable paths based upon search_path).
		 *
		 * The apply_ui_factor setup the QT_SCALE_FACTOR enviroment variable by getting the custom ui factor
		 * from the file appearance.conf.
		 *
		 * ATTENTION: this method MUST be called before the instantiation of the Application
		 * class otherwise the paths will not be configured correctly. */
		static void init(const QString &search_path, bool apply_ui_factor);

		//! \brief Returns the path to the "schemas" folder
		static QString getSchemasRootPath();

		/*! \brief Returns the path to a schema file under "schemas" folder.
		 * Since this method only operates over schemas folder there's no need to provide a file with
		 * extension because the method already appends the extension .sch automatically.
		 */
		template<typename ...args>
		static QString getSchemaFilePath(args... pth_elems)
		{
			return getFilePath(SchemaExt, SchemasRootPath, pth_elems...);
		}

		//! \brief Returns the path to the "tmp" folder in user's local storage
		static QString getTemporaryPath();

		//! \brief Returns the path to a temp file under "tmp" folder at user's local storage
		static QString getTemporaryFilePath(const QString &file);

		//! \brief Returns the path to the template "conf" folder in pgModeler's installation
		static QString getTmplConfigurationPath();

		/*! \brief Returns the path to the template file at template "conf" folder in pgModeler's installation
		 * This method will not append any extension to the file since this folder has several kinds of
		 * files inside it, so, the user must inform the file with its extension */
		template<typename ...args>
		static QString getTmplConfigurationFilePath(args... pth_elems)
		{
			return getFilePath("", TmplConfigurationPath, pth_elems...);
		}

		//! \brief Returns the path to the "conf" folder in user's local storage
		static QString getConfigurationsPath();

		/*! \brief Returns the path to a config file under "conf" folder at user's local storage.
		 * There's no need to specify the extension (.conf) since the method will automatically append it. */
		static QString getConfigurationFilePath(const QString &file);

		//! \brief Returns the path to the "samples" folder in pgModeler's installation
		static QString getSamplesPath();

		//! \brief Returns the path to the "lang" folder in pgModeler's installation
		static QString getLanguagesPath();

		//! \brief Returns the path to the "plugins" folder in pgModeler's installation
		static QString getPluginsPath();

		//! \brief Returns the path to the "sql-highlight.conf" file in user's local storage
		static QString getSQLHighlightConfPath();

		//! \brief Returns the path to the "xml-highlight.conf" file in user's local storage
		static QString getXMLHighlightConfPath();

		//! \brief Returns the path to the "sch-highlight.conf" file in user's local storage
		static QString getSchHighlightConfPath();

		//! \brief Returns the path to the "pattern-highlight.conf" file in user's local storage
		static QString getPatternHighlightConfPath();

		//! \brief Returns the path to the "pgmodeler-ch" (crash handler) executable
		static QString getPgModelerCHandlerPath();

		//! \brief Returns the path to the "pgmodeler-cli" (command line interface) executable
		static QString getPgModelerCLIPath();

		//! \brief Returns the path to the "pgmodeler" (main application) executable
		static QString getPgModelerAppPath();

		//! \brief Returns the path to the "pgmodeler-se" executable
		static QString getPgModelerSchemaEditorPath();

		/*! \brief Returns the version of the tool excluding any suffix
		 * appended to the version number (e.g., alpha, beta, snapshot, etc) */
		static QString getPgModelerBaseVersion();

		/*! \brief Returns the param_name value in the specified configuration file.
		 *  Returns empty string when the config parameter or the file does not exist. */
		static QString getConfigParamFromFile(const QString &param_name, const QString &conf_file);

		friend class PgModelerPlugin;
		friend class PgModelerCliPlugin;
};

#endif
