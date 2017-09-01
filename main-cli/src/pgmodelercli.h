/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2017 - Raphael Araújo e Silva <raphael@pgmodeler.com.br>
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
\class PgModelerCLI
\brief Implements the operations export models whitout use the graphical interface
*/

#ifndef PGMODELER_CLI_H
#define PGMODELER_CLI_H

#include <QObject>
#include <QTextStream>
#include <QCoreApplication>
#include "exception.h"
#include "globalattributes.h"
#include "modelwidget.h"
#include "modelexporthelper.h"
#include "generalconfigwidget.h"
#include "connectionsconfigwidget.h"

class PgModelerCLI: public QApplication {
	private:
		Q_OBJECT

		XMLParser *xmlparser;

		//! \brief Export helper object
		ModelExportHelper export_hlp;

		//! \brief Reference database model
		DatabaseModel *model;

		//! \brief Graphical scene used to export the model to png
		ObjectsScene *scene;

		//! \brief Stores the configured connection
		Connection connection;

		//! \brief Loaded connections
		map<QString, Connection *> connections;

		//! \brief Connection configuration widget used to load available connections from file
		ConnectionsConfigWidget conn_conf;

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
		QStringList objs_xml;

		//! \brief Zoom to be applied onto the png export
		double zoom;

		//! \brief Option names constants
		static const QString INPUT,
		OUTPUT,
		EXPORT_TO_FILE,
		EXPORT_TO_PNG,
		EXPORT_TO_SVG,
		EXPORT_TO_DBMS,
		DROP_DATABASE,
		DROP_OBJECTS,
		PGSQL_VER,
		HELP,
		SHOW_GRID,
		SHOW_DELIMITERS,
		PAGE_BY_PAGE,
		IGNORE_DUPLICATES,
		IGNORE_ERROR_CODES,
		CONN_ALIAS,
		HOST,
		PORT,
		USER,
		PASSWD,
		INITIAL_DB,
		SILENT,
		LIST_CONNS,
		SIMULATE,
		FIX_MODEL,
		FIX_TRIES,
		ZOOM_FACTOR,
		USE_TMP_NAMES,
		DBM_MIME_TYPE,
		INSTALL,
		UNINSTALL,

		TAG_EXPR,
		END_TAG_EXPR,
		ATTRIBUTE_EXPR;

		//! \brief Parsers the options and executes the action specified by them
		void parseOptions(attribs_map &parsed_opts);

		//! \brief Shows the options menu
		void showMenu(void);

		//! \brief Returns if the specified options exists on short options map
		bool isOptionRecognized(QString &op, bool &accepts_val);

		//! \brief Initializes the options maps
		void initializeOptions(void);

		/*! \brief Extracts the xml defintions from the input model and store them on obj_xml list
		in order to be parsed by the recreateObjects() method */
		void extractObjectXML(void);

		//! \brief Recreates the objects from the obj_xml list fixing the creation order for them
		void recreateObjects(void);

		//! \brief Fix some xml attributes and remove unused tags
		void fixObjectAttributes(QString &obj_xml);

		/*! \brief Extracts the foreign key code for the specified table xml. The foreign keys
		are recreated after all the other objects */
		QStringList extractForeignKeys(QString &obj_xml);

		//! \brief Returns if the specified string contains some of relationship attributes
		bool containsRelAttributes(const QString &str);

		/*! \brief Install the .dbm file association in the mime database (default behaviour).
		The paramenter 'uninstall' is used to clean up any file association done previously. */
		void handleMimeDatabase(bool uninstall);

		/*! \brief Fixes the references to opertor classes and families by replacing tags like
		<opclass name="name"/> by <opclass signature="name USING index_method"/>. This method operates
		only over operator classes, indexes and constraints */
		void fixOpClassesFamiliesReferences(QString &obj_xml);

	public:
		PgModelerCLI(int argc, char **argv);
		~PgModelerCLI(void);
		int exec(void);

	private slots:
		void handleObjectAddition(BaseObject *);
		void updateProgress(int progress, QString msg);
		void handleObjectRemoval(BaseObject *object);
};

#endif
