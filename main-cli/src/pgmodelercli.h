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
\ingroup pgmodeler-cli
\class PgModelerCliApp
\brief Implements the operations export models whitout use the graphical interface
*/

#ifndef PGMODELER_CLI_APP_H
#define PGMODELER_CLI_APP_H

#include "application.h"
#include <QObject>
#include <QTextStream>
#include <QCoreApplication>
#include "exception.h"
#include "globalattributes.h"
#include "modelwidget.h"
#include "modelexporthelper.h"
#include "generalconfigwidget.h"
#include "connectionsconfigwidget.h"
#include "relationshipconfigwidget.h"
#include "generalconfigwidget.h"
#include "databaseimporthelper.h"
#include "modelsdiffhelper.h"

class PgModelerCliApp: public Application {
	private:
		Q_OBJECT

		XmlParser *xmlparser;

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
		map<QString, Connection *> connections;

		//! \brief Connection configuration widget used to load available connections from file
		ConnectionsConfigWidget *conn_conf;

		//! \brief Relationship configuration widget used to load custom relationship settings
		RelationshipConfigWidget *rel_conf;

		GeneralConfigWidget *general_conf;

		//! \brief Creates an standard out to handles QStrings
		static QTextStream out;

		//! \brief Stores the long option names. The boolean indicates if the option accepts a value
		map<QString, bool> long_opts;

		//! \brief Stores the short option names.
		attribs_map short_opts;

		//! \brief Stores the parsed options names and values.
		attribs_map parsed_opts;

		//! \brief Indicates if the cli must run in silent mode
		bool silent_mode;

		//! \brief Stores the xml code for the objects being fixed
		QStringList objs_xml,

		//! \brief Stores the object filters for reverse engineering
		obj_filters;

		//! \brief Zoom to be applied onto the png export
		double zoom;

		static const QRegExp PasswordRegExp;

		static const QString PasswordPlaceholder;

		//! \brief Option names constants
		static const QString Input,
		Output,
		InputDb,
		ExportToFile,
		ExportToPng,
		ExportToSvg,
		ExportToDbms,
		ExportToDict,
		ImportDb,
		Diff,
		DropDatabase,
		DropObjects,
		PgSqlVer,
		Help,
		ShowGrid,
		ShowDelimiters,
		PageByPage,
		IgnoreDuplicates,
		IgnoreErrorCodes,
		ConnAlias,
		Host,
		Port,
		User,
		Passwd,
		InitialDb,
		Silent,
		ListConns,
		Simulate,
		FixModel,
		FixTries,
		ZoomFactor,
		UseTmpNames,
		DbmMimeType,
		Install,
		Uninstall,
		SystemWide,
		NoIndex,
		Splitted,

		IgnoreImportErrors,
		ImportSystemObjs,
		ImportExtensionObjs,
		DebugMode,
		FilterObjs,
		OnlyMatching,
		KeepChildObjs,

		CompareTo,
		SaveDiff,
		ApplyDiff,
		NoDiffPreview,
		DropClusterObjs,
		RevokePermissions,
		DropMissingObjs,
		ForceDropColsConstrs,
		RenameDb,
		TruncOnColsTypeChange,
		NoSequenceReuse,
		NoCascadeDropTrunc,
		NoForceObjRecreation,
		NoUnmodObjRecreation,

		CreateConfigs,

		TagExpr,
		EndTagExpr,
		AttributeExpr,

		MsgFileAssociated,
		MsgNoFileAssociation;

		//! \brief Parsers the options and executes the action specified by them
		void parseOptions(attribs_map &parsed_opts);

		//! \brief Shows the options menu
		void showMenu();

		//! \brief Returns if the specified options exists on short options map
		bool isOptionRecognized(QString &op, bool &accepts_val);

		//! \brief Initializes the options maps
		void initializeOptions();

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
		void handleMimeDatabase(bool uninstall, bool system_wide);

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

		void printMessage(const QString &msg);

		void handleLinuxMimeDatabase(bool uninstall, bool system_wide);
		void handleWindowsMimeDatabase(bool uninstall, bool system_wide);

		void createConfigurations();

	public:
		PgModelerCliApp(int argc, char **argv);
		virtual ~PgModelerCliApp();
		int exec();

	private slots:
		void handleObjectAddition(BaseObject *);
		void updateProgress(int progress, QString msg, ObjectType = ObjectType::BaseObject);
		void printIgnoredError(QString err_cod, QString err_msg, QString cmd);
		void handleObjectRemoval(BaseObject *object);
};

#endif
