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
\ingroup libpgmodeler_ui
\class ExportHelper
\brief Implements the base operations to export models
*/

#ifndef MODEL_EXPORT_HELPER_H
#define MODEL_EXPORT_HELPER_H

#include "modelwidget.h"
#include "dbconnection.h"

class ModelExportHelper: public QObject {
	private:
		Q_OBJECT

		int progress, sql_gen_progress;

	public:
		ModelExportHelper(QObject *parent = 0) : QObject(parent){ progress=0; }

		//! \brief Exports the model to a named SQL file. The PostgreSQL version syntax must be specified.
		void exportToSQL(DatabaseModel *db_model, const QString &filename, const QString &pgsql_ver);

		/*! \brief Exports the model to a named PNG image. The boolean parameters controls the grid exhibition
		as well the page delimiters on the output image */
		void exportToPNG(ObjectsScene *scene, const QString &filename, bool show_grid, bool show_delim);

		/*! \brief Exports the model directly to the DBMS. A valid connection must be specified. The PostgreSQL
		version is optional, since the helper identifies the version from the server. The boolean parameter
		make the helper to ignore object duplicity errors */
		void exportToDBMS(DatabaseModel *db_model, DBConnection &conn, const QString &pgsql_ver, bool ignore_dup);

	signals:
		//! \brief This singal is emitted whenever the export progress changes
		void s_progressUpdated(int progress, QString object_id);

	private slots:
		//! \brief Updates the exporting progress with the internal progress of sql generation of objects
		void updateProgress(int progress, QString object_id, unsigned);
};

#endif
