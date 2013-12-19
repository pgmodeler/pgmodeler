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
\class ModelExportHelper
\brief Implements the base operations to export models (forward engineering)
*/

#ifndef MODEL_EXPORT_HELPER_H
#define MODEL_EXPORT_HELPER_H

#include "modelwidget.h"
#include "connection.h"

class ModelExportHelper: public QObject {
	private:
		Q_OBJECT

		//! \brief  Stores the total progress
		int progress,

		//! \brief  Stores the sql generation progress
		sql_gen_progress;

		/*! \brief Indicates if the database was created on the server (only dbms export).
		This attribute is used to drop the database from server */
		bool db_created;

		//! \brief PostgreSQL version used by the exporter (only in thread mode)
		QString pgsql_ver;

		//! \brief Indicates to the exporter to ignore object duplicity (only in thread mode)
		bool ignore_dup,

		//! \brief Indicates to the exporter to run as a simulator[model validation] (only in thread mode)
		simulate,

		//! \brief Indicates to the exporter to drop database before export
		drop_db,

		//! \brief Indicates if the exporting thread was canceled by the user (only in thread mode)
		export_canceled;

		//! \brief Database model used as reference on export operation (only in thread mode)
		DatabaseModel *db_model;

		//! \brief Database connection used to export data to DBMS (only in thread mode)
		Connection *connection;

		/*! \brief Indicates which role / tablespaces were created on server (only dbms export).
		This attribute is used to drop the created roles / tablespaces from server */
		map<ObjectType, int> created_objs;

		//! \brief Stores the current state of ALTER command generation for table columns/constraints
		map<Table *, bool> alter_cmds_status;

		//! \brief Saves the current state of ALTER command generaton for table columns/constraints
		void saveGenAtlerCmdsStatus(DatabaseModel *db_model);

		//! \brief Retores the previous ALTER command generation state for table columns/constraints
		void restoreGenAtlerCmdsStatus(void);

		//! \brief Revert the dbms export process, removing the created database, roles and tablespaces
		void undoDBMSExport(DatabaseModel *db_model, Connection &conn);

	protected:
		//! \brief Configures the DBMS export params before start the export thread (only in thread mode)
		void setExportToDBMSParams(DatabaseModel *db_model, Connection *conn, const QString &pgsql_ver="", bool ignore_dup=false, bool drop_db=false, bool simulate=false);

	public:
		ModelExportHelper(QObject *parent = 0);

		//! \brief Exports the model to a named SQL file. The PostgreSQL version syntax must be specified.
		void exportToSQL(DatabaseModel *db_model, const QString &filename, const QString &pgsql_ver);

		/*! \brief Exports the model to a named PNG image. The boolean parameters controls the grid exhibition
		as well the page delimiters on the output image. The zoom parameter controls the zoom applied to the viewport
		before draw it on the pixmap */
		void exportToPNG(ObjectsScene *scene, const QString &filename, float zoom, bool show_grid, bool show_delim);

		/*! \brief Exports the model directly to the DBMS. A valid connection must be specified. The PostgreSQL
		version is optional, since the helper identifies the version from the server. The boolean parameter
		make the helper to ignore object duplicity errors */
		void exportToDBMS(DatabaseModel *db_model, Connection conn, const QString &pgsql_ver="", bool ignore_dup=false, bool drop_db=false, bool simulate=false);

		/*! \brief When the execution of the instance of this class is in another thread instead of main app
		thread puts the parent thread to sleep for [msecs] ms to give time to external operationsto be correctly
		finished before completely quit the thread itself otherwise the method don't do anything. */
		void sleepThread(unsigned msecs);

	signals:
		//! \brief This singal is emitted whenever the export progress changes
		void s_progressUpdated(int progress, QString msg, ObjectType obj_type=BASE_OBJECT);

		//! \brief This signal is emited when the export has finished
		void s_exportFinished(void);

		//! \brief This signal is emited when the export has been cancelled
		void s_exportCanceled(void);

		//! \brief This signal is emited when the export has encountered a critical error (only in thread mode)
		void s_exportAborted(Exception e);

	protected slots:
		void exportToDBMS(void);
		void cancelExport(void);

	private slots:
		//! \brief Updates the exporting progress with the internal progress of sql generation of objects
		void updateProgress(int progress, QString object_id, unsigned obj_type);

	friend class ModelValidationHelper;
	friend class ModelExportForm;
};

#endif
