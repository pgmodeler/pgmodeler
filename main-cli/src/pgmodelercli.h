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

    /* Stores the executable's absolute path. This is used to switch the working dir
    to the executable path in order to avoid the cli to find it's dependency paths
    (./schemas, ./conf) */
    QString executable_dir;

		//Export helper object
		ModelExportHelper export_hlp;

		//Reference database model
		DatabaseModel *model;

		//Graphical scene used to export the model to png
		ObjectsScene *scene;

		//Stores the configured connection
		DBConnection connection;

		//Loaded connections
		map<QString, DBConnection *> connections;

		//Connection configuration widget used to load available connections from file
		ConnectionsConfigWidget conn_conf;

		//Creates an standard out to handles QStrings
		static QTextStream out;

		//Stores the long option names. The boolean indicates if the option accepts a value
		map<QString, bool> long_opts;

		//Stores the short option names.
		map<QString, QString> short_opts;

		//Stores the parsed options names and values.
		map<QString, QString> parsed_opts;

		//Indicates if the cli must run in silent mode
		bool silent_mode;

		//Option names constants
		static QString INPUT,
		OUTPUT,
		EXPORT_TO_FILE,
		EXPORT_TO_PNG,
		EXPORT_TO_DBMS,
		PGSQL_VER,
		HELP,
		SHOW_GRID,
		SHOW_DELIMITERS,
		IGNORE_DUPLICATES,
		CONN_ALIAS,
		HOST,
		PORT,
		USER,
		PASSWD,
		INITIAL_DB,
		SILENT,
		LIST_CONNS;

		//Parsers the options and executes the action specified by them
		void parserOptions(map<QString, QString> &parsed_opts);

		//Shows the options menu
		void showMenu(void);

		//Returns if the specified options exists on short options map
		bool isOptionRecognized(QString &op, bool &accepts_val);

		//Initializes the options maps
		void initializeOptions(void);

	public:
		PgModelerCLI(int argc, char **argv);
		~PgModelerCLI(void);
		int exec(void);

	private slots:
		void handleObjectAddition(BaseObject *);
		void updateProgress(int progress, QString msg);
};

#endif
