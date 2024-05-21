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
\ingroup libcli
\class PgModelerCliApp
\brief Implements a set of operations that can be used without the graphical interface
*/

#ifndef PGMODELER_CLI_APP_H
#define PGMODELER_CLI_APP_H

#include "application.h"
#include <QObject>
#include <QTextStream>
#include <QCoreApplication>
#include "tools/modelexporthelper.h"
#include "settings/generalconfigwidget.h"
#include "settings/connectionsconfigwidget.h"
#include "settings/relationshipconfigwidget.h"
#include "settings/generalconfigwidget.h"
#include "tools/databaseimporthelper.h"
#include "tools/modelsdiffhelper.h"
#include "pgmodelercliplugin.h"

class __libcli PgModelerCliApp: public Application {
	private:
		Q_OBJECT

		XmlParser *xmlparser;

		qint64 buffer_size;

		bool has_fix_log,
		fix_model,
		upd_mime,
		import_db,
		diff,
		create_configs,
		list_conns,
		list_plugins,
		plugin_op,
		export_op;

		//! \brief Holds the pgModeler version in which the model was construted (used by the fix operation)
		QString model_version;

		//! \brief Export helper object
		ModelExportHelper *export_hlp;

		//! \brief Import helper object
		DatabaseImportHelper *import_hlp;

		//! \brief Diff helper object
		ModelsDiffHelper *diff_hlp;

		//! \brief Reference database model
		DatabaseModel *model;

		//! \brief Graphical scene used to export the model to png
		ObjectsScene *scene;

		//! \brief Stores the configured connection
		Connection connection,

		//! \brief Stores the extra configured connection (only for diff)
		extra_connection;

		//! \brief Loaded connections
		std::map<QString, Connection *> connections;

		//! \brief Connection configuration widget used to load available connections from file
		ConnectionsConfigWidget *conn_conf;

		//! \brief Relationship configuration widget used to load custom relationship settings
		RelationshipConfigWidget *rel_conf;

		GeneralConfigWidget *general_conf;

		//! \brief Stores the instances of loaded plugins
		QList<PgModelerCliPlugin *> plugins, plug_exec_order;

		//! \brief Creates an standard out to handles QStrings
		inline static QTextStream out {stdout};

		//! \brief Stores the parsed options names and values.
		attribs_map parsed_opts;

		//! \brief Indicates if the cli must run in silent mode
		bool silent_mode;

		//! \brief Store the error stack related to plugins loading
		QString plugin_load_errors;

		//! \brief Stores the xml code for the objects being fixed
		QStringList objs_xml,

		//! \brief Stores the object filters for reverse engineering
		obj_filters;

		//! \brief Zoom to be applied onto the png export
		double zoom;

		//! \brief Start date used for filter changelog of the input database model (partial diff)
		QDateTime start_date,

		//! \brief End date used for filter changelog of the input database model (partial diff)
		end_date;

		/*! \brief Stores the member of role names that appear in deprecated tags <roles ... role-type="refer">
		 * This map is used to reconfigure the role memberships after all objects are created */
		std::map<QString, QStringList> member_roles;

		//! \brief Stores the changelog of the model that is being fixed to reproduce it in the output model
		QString changelog;

		inline static const QRegularExpression PasswordRegExp { "(password)(=)(.)*( )" };

		inline static const QString PasswordPlaceholder { "password=******" };

		//! \brief Parsers the options and executes the action specified by them
		void parseOptions(attribs_map &parsed_opts);

		//! \brief Returns if the specified options exists on short options map
		bool isOptionRecognized(QString &op, bool &accepts_val);

		//! \brief Loads the input model and perform all tasks needed to configure the graphical objects
		void loadModel();

		/*! \brief Extracts the xml defintions from the input model and store them on obj_xml list
		in order to be parsed by the recreateObjects() method */
		void extractObjectXML();

		//! \brief Recreates the objects from the obj_xml list fixing the creation order for them
		void recreateObjects();

		//! \brief Fix some xml attributes and remove unused tags
		void fixObjectAttributes(QString &obj_xml);

		/*! \brief Extracts the foreign key code for the specified table xml. The foreign keys
		are recreated after all the other objects */
		QStringList extractForeignKeys(QString &obj_xml);

		//! \brief Returns if the specified string contains some of relationship attributes
		bool containsRelAttributes(const QString &str);

		/*! \brief Install the .dbm file association in the mime database (default behaviour).
		The paramenter 'uninstall' is used to clean up any file association done previously. */
		void handleMimeDatabase(bool uninstall, bool system_wide, bool force);

		/*! \brief Fixes the references to opertor classes and families by replacing tags like
		<opclass name="name"/> by <opclass signature="name USING index_method"/>. This method operates
		only over operator classes, indexes and constraints */
		void fixOpClassesFamiliesReferences(QString &obj_xml);

		void fixModel();
		void exportModel();
		void importDatabase();
		void diffModelDatabase();
		void updateMimeType();
		void configureConnection(bool extra_conn);
		void importDatabase(DatabaseModel *model, Connection conn);

		void handleLinuxMimeDatabase(bool uninstall, bool system_wide, bool force);
		void handleWindowsMimeDatabase(bool uninstall, bool system_wide, bool force);
		void createConfigurations();
		void listConnections();
		void loadPlugins();
		void listPlugins();
		bool isPluginOptsValid(const PgModelerCliPlugin *plugin);

		/*! \brief Determines the execution order of the plugins by reading the
		 *  list of options provided. This method also returns the number of
		 *  plugins that are set to run custom CLI operations (standalone). */
		int definePluginsExecOrder(const attribs_map &opts);

		//! \brief Execute all activated plugins pre operations
		void runPluginsPreOperations();

		//! \brief Execute all activated plugins custom operations
		void runPluginsOperations();

		//! \brief Execute all activated plugins post operations
		void runPluginsPostOperations();

		int exec();

	public:
		//! \brief Option names constants
		inline static const QString AllChildren {"all"},
		Input {"--input"},
		Output {"--output"},
		InputDb {"--input-db"},
		ExportToFile {"--export-to-file"},
		ExportToPng {"--export-to-png"},
		ExportToSvg {"--export-to-svg"},
		ExportToDbms {"--export-to-dbms"},
		ExportToDict {"--export-to-dict"},
		ImportDb {"--import-db"},
		NoIndex {"--no-index"},
		Split {"--split"},
		Markdown {"--markdown"},
		DependenciesSql {"--dependencies"},
		ChildrenSql {"--children"},
		GroupByType {"--group-by-type"},
		GenDropScript {"--gen-drop-script"},
		Diff {"--diff"},
		DropDatabase {"--drop-database"},
		DropObjects {"--drop-objects"},
		PgSqlVer {"--pgsql-ver"},
		Help {"--help"},
		ShowGrid {"--show-grid"},
		ShowDelimiters {"--show-delimiters"},
		PageByPage {"--page-by-page"},
		OverrideBgColor {"--override-bg-color"},
		IgnoreDuplicates {"--ignore-duplicates"},
		IgnoreErrorCodes {"--ignore-error-codes"},
		ConnAlias {"--conn-alias"},
		Host {"--host"},
		Port {"--port"},
		User {"--user"},
		Passwd {"--passwd"},
		InitialDb {"--initial-db"},
		Silent {"--silent"},
		ListConns {"--list-conns"},
		Simulate {"--simulate"},
		FixModel {"--fix-model"},
		FixTries {"--fix-tries"},
		ZoomFactor {"--zoom"},
		UseTmpNames {"--use-tmp-names"},
		DbmMimeType {"--dbm-mime-type"},
		Install {"install"},
		Uninstall {"uninstall"},
		SystemWide {"--system-wide"},
		IgnoreImportErrors {"--ignore-errors"},
		ImportSystemObjs {"--import-sys-objs"},
		ImportExtensionObjs {"--import-ext-objs"},
		DebugMode {"--debug-mode"},
		FilterObjects {"--filter-objects"},
		MatchByName {"--match-by-name"},
		ForceChildren {"--force-children"},
		OnlyMatching {"--only-matching"},
		CommentsAsAliases {"--comments-as-aliases"},
		PartialDiff {"--partial"},
		Force {"--force"},
		StartDate {"--start-date"},
		EndDate {"--end-date"},
		CompareTo {"--compare-to"},
		SaveDiff {"--save"},
		ApplyDiff {"--apply"},
		NoDiffPreview {"--no-preview"},
		DropClusterObjs {"--drop-cluster-objs"},
		RevokePermissions {"--revoke-perms"},
		DropMissingObjs {"--drop-missing"},
		ForceDropColsConstrs {"--force-drop-cols"},
		RenameDb {"--rename-db"},
		NoSequenceReuse {"--no-sequence-reuse"},
		NoCascadeDrop {"--no-cascade"},
		RecreateUnmod {"--recreate-unmod"},
		ReplaceModified {"--replace-mod"},
		CreateConfigs {"--create-configs"},
		MissingOnly {"--missing-only"},
		IgnoreFaultyPlugins {"--ignore-faulty"},
		ListPlugins {"--list-plugins"},

		TagExpr {"<%1"},
		EndTagExpr {"</%1"},
		AttributeExpr {"(%1)( )*(=)(\")(\\w|\\d|,|\\.|\\&|\\;|\\)|\\(|\\-| )+(\")"},

		ModelFixLog {"model_fix.log"},

		MsgFileAssociated {
			QString(QT_TR_NOOP("Database model files (*%1) are already associated with pgModeler! Try using the option `%2' to install the file association anyway.")).arg(GlobalAttributes::DbModelExt, Force)
		},

		MsgNoFileAssociation {
			QString(QT_TR_NOOP("There is no file association related to pgModeler and *%1 files! Try using the option `%2' to uninstall the file association anyway.")).arg(GlobalAttributes::DbModelExt, Force)
		};

		PgModelerCliApp(int argc, char **argv);

		virtual ~PgModelerCliApp();

		//! \brief Shows the options menu
		void showMenu();

					 //! \brief Shows the version info
		void showVersionInfo();

		/*! \brief Prints to the stdout the provided text appending a \n on the string
		 * even if the silent mode is active. */
		void printText(const QString &txt = "");

		//! \brief Prints to the stdout only if the silent mode is not active
		void printMessage(const QString &txt = "");

		//! \brief Returns the options parsed when calling the application
		attribs_map getParsedOptions();

		//! \brief Returns a single parsed option value
		QString getParsedOptValue(const QString &opt);

		//! \brief Replaces the value of a single parsed option
		void setParsedOptValue(const QString &opt, const QString &value);

	private slots:
		void handleObjectAddition(BaseObject *);
		void updateProgress(int progress, QString msg, ObjectType = ObjectType::BaseObject);
		void printIgnoredError(QString err_cod, QString err_msg, QString cmd);
		void handleObjectRemoval(BaseObject *object);

	private:
		//! \brief Stores the long option names. The boolean indicates if the option accepts a value
		inline static std::map<QString, bool> long_opts {
			{ Input, true }, { Output, true }, { InputDb, true },
			{ ExportToFile, false },	{ ExportToPng, false },	{ ExportToSvg, false },
			{ ExportToDbms, false },	{ ImportDb, false },	{ Diff, false },
			{ DropDatabase, false },	{ DropObjects, false },	{ PgSqlVer, true },
			{ Help, false },	{ ShowGrid, false },	{ ShowDelimiters, false },
			{ PageByPage, false },	{ IgnoreDuplicates, false },	{ OverrideBgColor, false },
			{ IgnoreErrorCodes, true }, { ConnAlias, true },	{ Host, true },	{ Port, true },
			{ User, true },	{ Passwd, true },	{ InitialDb, true },
			{ ListConns, false },	{ Simulate, false },	{ FixModel, false },
			{ FixTries, true },	{ ZoomFactor, true },	{ UseTmpNames, false },
			{ DbmMimeType, true },	{ IgnoreImportErrors, false },	{ ImportSystemObjs, false },
			{ ImportExtensionObjs, false },	{ FilterObjects, true },	{ ForceChildren, true },
			{ OnlyMatching, false },	{ MatchByName, false },	{ DebugMode, false },
			{ PartialDiff, false },	{ StartDate, true },	{ EndDate, true },
			{ CompareTo, true },	{ SaveDiff, false },	{ ApplyDiff, false },
			{ NoDiffPreview, false },	{ DropClusterObjs, false },	{ RevokePermissions, false },
			{ DropMissingObjs, false },	{ ForceDropColsConstrs, false },	{ RenameDb, false },
			{ NoSequenceReuse, false },	{ NoCascadeDrop, false },
			{ RecreateUnmod, false }, { ReplaceModified, false },	{ ExportToDict, false },
			{ NoIndex, false },	{ Split, false },	{ SystemWide, false },
			{ CreateConfigs, false }, { Force, false }, { MissingOnly, false },
			{ DependenciesSql, false }, { ChildrenSql, false }, { GenDropScript, false },
			{ GroupByType, false }, { CommentsAsAliases, false }, { IgnoreFaultyPlugins, false },
			{ ListPlugins, false }, { Markdown, false }
		};

		//! \brief Stores the short option names.
		inline static attribs_map short_opts {
			{ Input, "-if" },		{ Output, "-of" },	{ InputDb, "-id" },
			{ ExportToFile, "-ef" },	{ ExportToPng, "-ep" },	{ ExportToSvg, "-es" },
			{ ExportToDbms, "-ed" },	{ ExportToDict, "-ec" },	{ ImportDb, "-im" },
			{ Diff, "-df" },	{ DropDatabase, "-dd" },	{ DropObjects, "-do" },
			{ PgSqlVer, "-v" },	{ Help, "-h" },	{ ShowGrid, "-sg" },
			{ ShowDelimiters, "-sl" },	{ PageByPage, "-pp" },
			{ IgnoreDuplicates, "-ir" }, { OverrideBgColor, "-oc" },
			{ IgnoreErrorCodes, "-ic" },	{ ConnAlias, "-ca" },	{ Host, "-H" },
			{ Port, "-p" },	{ User, "-u" },	{ Passwd, "-w" },
			{ InitialDb, "-D" },	{ Silent, "-s" },	{ ListConns, "-lc" },
			{ Simulate, "-sm" },	{ FixModel, "-fm" },	{ FixTries, "-ft" },
			{ ZoomFactor, "-zf" },	{ UseTmpNames, "-tn" },	{ DbmMimeType, "-mt" },
			{ IgnoreImportErrors, "-ie" },	{ ImportSystemObjs, "-is" },	{ ImportExtensionObjs, "-ix" },
			{ FilterObjects, "-fo" },	{ MatchByName, "-mn" },	{ ForceChildren, "-fc" },
			{ OnlyMatching, "-om" },	{ DebugMode, "-d" },	{ PartialDiff, "-pd" },
			{ StartDate, "-st" },	{ EndDate, "-et" },	{ CompareTo, "-ct" },
			{ SaveDiff, "-sd" },	{ ApplyDiff, "-ad" },	{ NoDiffPreview, "-np" },
			{ DropClusterObjs, "-dc" },	{ RevokePermissions, "-rv" },	{ DropMissingObjs, "-dm" },
			{ ForceDropColsConstrs, "-fd" },	{ RenameDb, "-rn" },
			{ NoSequenceReuse, "-ns" },	{ NoCascadeDrop, "-nd" },
			{ RecreateUnmod, "-ru" }, { ReplaceModified, "-rm" },	{ NoIndex, "-ni" },	{ Split, "-sp" },
			{ SystemWide, "-sw" },	{ CreateConfigs, "-cc" }, { Force, "-ff" },
			{ MissingOnly, "-mo" }, { DependenciesSql, "-ds" }, { ChildrenSql, "-cs" },
			{ GroupByType, "-gt" },	{ GenDropScript, "-gd" }, { CommentsAsAliases, "-cl" },
			{ IgnoreFaultyPlugins, "-ip" }, { ListPlugins, "-lp" }, { Markdown, "-md" }
		};

		//! \brief Stores the accepted options by the different operations
		inline static std::map<QString, QStringList> accepted_opts {
			{{ Attributes::Connection }, { ConnAlias, Host, Port, User, Passwd, InitialDb }},
			{{ ExportToFile }, { Input, Output, PgSqlVer, Split, DependenciesSql, ChildrenSql, GroupByType, GenDropScript }},
			{{ ExportToPng },  { Input, Output, ShowGrid, ShowDelimiters, PageByPage, ZoomFactor, OverrideBgColor }},
			{{ ExportToSvg },  { Input, Output, ShowGrid, ShowDelimiters }},
			{{ ExportToDict }, { Input, Output, Split, NoIndex, Markdown }},

			{{ ExportToDbms }, { Input, PgSqlVer, IgnoreDuplicates, IgnoreErrorCodes,
														DropDatabase, DropObjects, Simulate, UseTmpNames, Force }},

			{{ ImportDb }, { InputDb, Output, IgnoreImportErrors, ImportSystemObjs, ImportExtensionObjs,
												FilterObjects, OnlyMatching, MatchByName, ForceChildren, DebugMode, ConnAlias,
												Host, Port, User, Passwd, InitialDb, CommentsAsAliases }},

			{{ Diff }, { Input, PgSqlVer, IgnoreDuplicates, IgnoreErrorCodes, CompareTo, PartialDiff, Force,
									 StartDate, EndDate, SaveDiff, ApplyDiff, NoDiffPreview, DropClusterObjs, RevokePermissions,
									 DropMissingObjs, ForceDropColsConstrs, RenameDb, NoCascadeDrop,
									 NoSequenceReuse, RecreateUnmod, ReplaceModified }},

			{{ DbmMimeType }, { SystemWide, Force }},
			{{ FixModel },	{ Input, Output, FixTries }},
			{{ ListConns }, { }},
			{{ CreateConfigs }, { MissingOnly, Force }},
			{{ ListPlugins }, { IgnoreFaultyPlugins }}
		};

		/* We make the main() a friend function of PgModelerCliApp just to
		 * allow it to call exec() that is a private function.
		 *
		 * This will avoid the exec() method to be called from within
		 * plugins which is not desirable. */
		friend int main(int, char **);
};

#endif
